# MonkeyCAM worker development and deployment

The Node worker accepts jobs from the web client, uses Google Cloud Pub/Sub for
the queue, runs the MonkeyCAM CLI, and stores results in Google Cloud. Production
runs on Cloud Run.

See the repository [development guide](../../DEVELOPING.md) for the full path
from engine validation through client deployment.

## Prerequisites

- Node.js 24.11.1 (`fnm use` reads this directory's `.node-version`).
- A native MonkeyCAM build for local development.
- Docker Desktop and the Google Cloud CLI for deployment.
- Access to the `monkeycam-web-app` Google Cloud project.

Use Application Default Credentials for local calls to Cloud Storage, Datastore,
and Pub/Sub:

```sh
gcloud auth application-default login
```

## Run locally

First build and test the engine from the repository root:

```sh
./engine/build-native.sh
```

Then start the worker:

```sh
cd webapp/worker
fnm use
npm ci
npm run check
NODE_ENV=development \
  monkeycamPath="$(cd ../../engine/build-dev/cli && pwd)" \
  npm start
```

The worker listens on port 8080. Check it from another terminal:

```sh
curl -fsS http://localhost:8080/
```

The expected response is `OK`. `monkeycamPath` is a directory, not the binary
path; the worker appends `MonkeyCAM`.

Keep `NODE_ENV=development` set for local work. It makes newly submitted jobs use
`MonkeyCAM-Job-Queue-Test`; without it, the worker publishes to the production
queue. Running the web client locally is covered in the
[client guide](../client/README.md#run-locally).

The `/pubsub` endpoint processes pushed jobs. Do not point the production
subscription at a laptop or tunnel for routine development: doing so redirects
all production work. Provision a separate push subscription on the test topic if
end-to-end local push testing is needed.

## Validate the worker

The worker currently has syntax checks but no automated unit-test suite:

```sh
cd webapp/worker
npm ci
npm run check
```

To validate the complete production container, first stage the Linux engine from
the repository root, then build the worker image:

```sh
./webapp/worker/stage-linux-engine.sh
docker build --platform=linux/amd64 \
  --tag monkeycam-worker-local \
  webapp/worker
```

The Dockerfile executes the staged engine's `--version` command while building,
so an incompatible or missing binary fails the build.

## Deploy to Cloud Run

These commands update the live worker. Run them from the repository root and
keep the explicit project and region values.

Build the current engine source as Linux/AMD64 and stage it into the worker build
context:

```sh
./webapp/worker/stage-linux-engine.sh
```

Submit the worker image to Cloud Build:

```sh
gcloud builds submit webapp/worker \
  --project=monkeycam-web-app \
  --tag=gcr.io/monkeycam-web-app/monkeycam-worker
```

Deploy it with the production service settings:

```sh
gcloud run deploy monkeycam-worker \
  --image=gcr.io/monkeycam-web-app/monkeycam-worker \
  --project=monkeycam-web-app \
  --region=us-central1 \
  --platform=managed \
  --allow-unauthenticated \
  --max-instances=1 \
  --concurrency=80 \
  --cpu=1 \
  --memory=512Mi \
  --timeout=300 \
  --service-account=monkeycam-web-app@appspot.gserviceaccount.com \
  --set-env-vars=monkeycamPath=/app/bin/linux
```

`monkeycamPath` selects the staged Linux binary. Cloud Run supplies `PORT`, which
the worker already respects.

Fetch the service URL returned by Cloud Run and check the deployed revision:

```sh
WORKER_URL="$(gcloud run services describe monkeycam-worker \
  --project=monkeycam-web-app \
  --region=us-central1 \
  --format='value(status.url)')"
curl -fsS "${WORKER_URL}/"

gcloud run services describe monkeycam-worker \
  --project=monkeycam-web-app \
  --region=us-central1 \
  --format='yaml(status.latestReadyRevisionName,status.traffic,spec.template.spec.containers[0].image)'
```

The staged `webapp/worker/bin/linux/MonkeyCAM` file is a generated deployment
artifact and is ignored by Git.

## Verify the Pub/Sub subscription

Production jobs arrive as wrapped JSON at `POST /pubsub`. The production objects
are:

- topic: `MonkeyCAM-Job-Queue`
- push subscription: `monkeycam-worker-push`
- acknowledgement deadline: 600 seconds
- expiration: never

Inspect the subscription after a worker deployment:

```sh
gcloud pubsub subscriptions describe monkeycam-worker-push \
  --project=monkeycam-web-app \
  --format='yaml(name,topic,pushConfig.pushEndpoint,ackDeadlineSeconds,expirationPolicy)'
```

The push endpoint may use a different valid `run.app` hostname than the one
returned by `gcloud run services describe`; do not change it solely to make the
strings match. Confirm that it targets the `monkeycam-worker` service and ends in
`/pubsub`.

Only when provisioning or repairing the subscription, obtain the service URL and
create it as follows:

```sh
WORKER_URL="$(gcloud run services describe monkeycam-worker \
  --project=monkeycam-web-app \
  --region=us-central1 \
  --format='value(status.url)')"

gcloud pubsub subscriptions create monkeycam-worker-push \
  --project=monkeycam-web-app \
  --topic=MonkeyCAM-Job-Queue \
  --push-endpoint="${WORKER_URL}/pubsub" \
  --ack-deadline=600 \
  --expiration-period=never
```

Do not add `--push-no-wrapper`; the handler expects the payload in
`body.message.data`.

Pub/Sub subscriptions expire after 31 days without subscriber activity by
default unless `--expiration-period=never` is set. To investigate a disappeared
subscription, search Logs Explorer for:

```text
protoPayload.methodName="google.pubsub.v1.Subscriber.DeleteSubscription"
```

`InternalExpireInactiveSubscription` identifies automatic expiration. Otherwise,
inspect the audit-log principal for the user or service account that deleted it.
