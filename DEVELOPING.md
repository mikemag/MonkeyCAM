# Developing MonkeyCAM

This is the starting point for building, testing, and deploying the repository.
Detailed commands live with each component:

| Component | Development guide | Deployment target |
| --- | --- | --- |
| C++ engine | [engine/README.md](engine/README.md) | Linux binary bundled into the worker |
| Node worker | [webapp/worker/README.md](webapp/worker/README.md) | Google Cloud Run |
| React client | [webapp/client/README.md](webapp/client/README.md) | Google App Engine |

The engine can be developed independently of the web app. The usual workflow is
to make and validate engine changes first, then package that exact source into the
worker only when it is ready to deploy.

## Prerequisites

- CMake 3.22 or newer, a C++17 compiler, and Python 3 for native engine tests.
- Docker Desktop for the reproducible Linux build used by Cloud Run. The Docker
  context must point at a running Docker engine.
- Node.js 24.11.1 for the worker and client. Their `.node-version` files let
  `fnm use` select it.
- The Google Cloud CLI for deployment, authenticated to an account with access
  to the `monkeycam-web-app` project.

The repository's production project is `monkeycam-web-app`. Every command in
these guides supplies `--project=monkeycam-web-app` deliberately; do not rely on
the active `gcloud` project.

Check Docker before starting a container build:

```sh
docker context show
docker version
```

If Docker Desktop is running but another context is selected, either switch with
`docker context use desktop-linux` or prefix a command with
`DOCKER_CONTEXT=desktop-linux` to override it for that command only.

## Engine iteration loop

From the repository root, build natively and run all C++ and G-Code regression
tests:

```sh
./engine/build-native.sh
```

The binary is written to `engine/build-dev/cli/MonkeyCAM`. For a focused rerun
of the maximum-cut-depth regression:

```sh
ctest --test-dir engine/build-dev \
  -R max-cut-depth-regression \
  --output-on-failure
```

The regression generates the snowboard, ski, and splitboard examples. It checks
that omitted or non-binding maximum cut depths preserve the established output,
and that active limits preserve path geometry and respect the requested depth.

To test the Linux build and inspect output from any set of JSON files:

```sh
OUTPUT_DIR="$(mktemp -d /tmp/monkeycam-output.XXXXXX)"
./engine/run-docker.sh \
  --board example/splitboard-def.json \
  --binding example/splitboard-binding-def.json \
  --machine engine/test/data/machine-max-cut-depth.json \
  --outdir "${OUTPUT_DIR}"
```

See the [engine guide](engine/README.md) for the snowboard and ski forms, how to
reuse a built image, native-build details, and sample-output comparison behavior.

## Web app iteration loop

The client development server listens on port 3000 and proxies API requests to a
local worker on port 8080. Build the engine natively first, then start the worker
and client in separate terminals as described in their guides:

- [Run and check the worker locally](webapp/worker/README.md#run-locally)
- [Run and check the client locally](webapp/client/README.md#run-locally)

Local worker calls use Google Cloud Storage, Datastore, and Pub/Sub. Use
Application Default Credentials and `NODE_ENV=development`; development jobs are
published to `MonkeyCAM-Job-Queue-Test`.

## Deployment order

A normal release is:

1. Run `./engine/build-native.sh` and resolve every failure.
2. Generate representative G-Code with `./engine/run-docker.sh` and inspect it.
3. Run `./webapp/worker/stage-linux-engine.sh` to build and stage the Linux engine.
4. Build and deploy the worker using the
   [Cloud Run instructions](webapp/worker/README.md#deploy-to-cloud-run).
5. Check the worker health endpoint and the existing Pub/Sub subscription.
6. Build and deploy the client using the
   [App Engine instructions](webapp/client/README.md#deploy-to-app-engine).
7. Submit a representative job through [MonkeyCAM.org](https://www.monkeycam.org)
   and inspect its overview and G-Code before considering the release complete.

Cloud deployments and production job submissions change live systems. Read the
component guide and inspect the project, service, image, and subscription names
before running those commands.

## Before opening a pull request

At minimum:

```sh
./engine/build-native.sh

cd webapp/worker
npm ci
npm run check

cd ../client
npm ci
npm run test:ci
npm run build
```

Only run the web commands for components affected by the change. Return to the
repository root between unrelated examples in the component guides.
