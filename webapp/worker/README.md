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

From webapp/worker, replace the placeholder values and build the image:

```sh
gcloud builds submit \
  --project=PROJECT_ID \
  --tag=gcr.io/PROJECT_ID/WORKER_IMAGE .
```

## Deploy to Cloud Run

Deploy the built image to Cloud Run, from webapp/worker:

```sh
gcloud run deploy WORKER_SERVICE \
  --image=gcr.io/PROJECT_ID/WORKER_IMAGE \
  --project=PROJECT_ID \
  --region=REGION \
  --platform=managed \
  --allow-unauthenticated \
  --max-instances=1 \
  --service-account=SERVICE_ACCOUNT_EMAIL \
  --set-env-vars=monkeycamPath=/app/bin/linux
```

The `monkeycamPath` environment variable ensures the worker uses the Linux binary bundled
in the container. Cloud Run automatically sets the `PORT` environment variable, which the
worker already respects.

## Configure the Pub/Sub push subscription

Production jobs are delivered to the worker through a Pub/Sub push subscription. The
worker receives Pub/Sub's default wrapped JSON message format at `POST /pubsub`.

Replace the placeholder values below, then create the subscription:

```sh
gcloud pubsub subscriptions create SUBSCRIPTION_ID \
  --project=PROJECT_ID \
  --topic=TOPIC_ID \
  --push-endpoint=https://CLOUD_RUN_SERVICE_URL/pubsub \
  --ack-deadline=600 \
  --expiration-period=never
```

Do not add `--push-no-wrapper`; the handler expects the message payload in
`body.message.data`.

Verify the subscription:

```sh
gcloud pubsub subscriptions describe SUBSCRIPTION_ID \
  --project=PROJECT_ID
```

If an existing subscription has the default expiration policy, make it persistent:

```sh
gcloud pubsub subscriptions update SUBSCRIPTION_ID \
  --project=PROJECT_ID \
  --expiration-period=never
```

Pub/Sub subscriptions expire after 31 days without subscriber activity by default.
Successful pushes or subscription configuration changes restart the inactivity clock.
To determine why a subscription disappeared, use Logs Explorer with:

```
protoPayload.methodName="google.pubsub.v1.Subscriber.DeleteSubscription"
```

An entry identifying `InternalExpireInactiveSubscription` indicates automatic
expiration. Otherwise, inspect the audit-log principal to identify the user or service
account that deleted it.
