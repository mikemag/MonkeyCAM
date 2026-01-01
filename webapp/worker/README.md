# Running Locally (osX)

## Build the engine and copy the binary

```
cd engine
cmake .
make -j
cp cli/MonkeyCAM ../webapp/worker/bin/osx
```

## Running the worker

- `cd webapp/worker`
- `fnm use`
- `npm install`
- `npm start`
  - Starts the worker listening on port 8080.
  - The direct endpoints use test or prod queue based on NODE_ENV.
  - The pubsub endpoint will only be hit if we redirect the push from google using ngrok.
    - `ngrok http 8080`
    - Grab the forwarding endpoint and update the pubsub push to use that and it will work.
      - Note that the local worker will process *all* prod requests while this is changed.
      - TODO: setup push for the test queue and use that for testing instead.


# Cloud Run deployment for the worker

This directory contains a Dockerfile that packages the worker for Google Cloud Run.

## Prerequisites

```
cd engine
docker-build.sh
docker create --name monkeycam-build monkeycam-engine-builder
docker cp monkeycam-build:/opt/monkeycam ./monkeycam-dist
docker rm monkeycam-build
```

The `./monkeycam-dist/bin` directory will contain the `MonkeyCAM` binary and supporting files.

```
cp monkeycam-dist/bin/MonkeyCAM webapp/worker/bin/linux
```

## Build the image

From webapp/worker:

```sh
gcloud builds submit --project monkeycam-web-app --tag gcr.io/monkeycam-web-app/monkeycam-worker .
```

## Deploy to Cloud Run

Deploy the built image to Cloud Run, from webapp/worker:

```sh
gcloud run deploy monkeycam-worker \
  --image gcr.io/monkeycam-web-app/monkeycam-worker \
  --project monkeycam-web-app \
  --region us-central1 \
  --platform managed \
  --allow-unauthenticated \
  --max-instances=1 \
  --service-account monkeycam-web-app@appspot.gserviceaccount.com \
  --set-env-vars monkeycamPath=/app/bin/linux
```

The `monkeycamPath` environment variable ensures the worker uses the Linux binary bundled
in the container. Cloud Run automatically sets the `PORT` environment variable, which the
worker already respects.
