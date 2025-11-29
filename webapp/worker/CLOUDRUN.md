# Cloud Run deployment for the worker

This directory contains a Dockerfile that packages the worker for Google Cloud Run.

## Prerequisites

- A Linux build of the `MonkeyCAM` binary located at `webapp/worker/bin/linux/MonkeyCAM`. See instructions in the repo README.md.
- `gcloud` CLI installed and authenticated to the target project.

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
