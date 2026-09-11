# MonkeyCAM web client development and deployment

The React client provides the MonkeyCAM.org UI. Its production static build is
served by Google App Engine.

See the repository [development guide](../../DEVELOPING.md) for the full path
from engine validation through deployment.

## Run locally

Start a local worker on port 8080 first, following the
[worker guide](../worker/README.md#run-locally). Then, in another terminal:

```sh
cd webapp/client
fnm use
npm ci
npm start
```

The development server listens on port 3000. The `proxy` setting in
`package.json` sends API calls to `http://localhost:8080`.

## Test and build

Run the test command once rather than entering watch mode, then produce the exact
static bundle App Engine will upload:

```sh
cd webapp/client
fnm use
npm ci
npm run test:ci
npm run build
```

There are no client test files at present; `test:ci` is configured to succeed in
that case and will run the suite once tests are added. The production build is
the current compile and lint validation. Its warnings should be reviewed even
when the command succeeds.

## Deploy to App Engine

These commands update the live web client. Build immediately before deploying so
the `build` directory matches the source under review:

```sh
cd webapp/client
fnm use
npm ci
npm run test:ci
npm run build
gcloud app deploy app.yaml --project=monkeycam-web-app
```

After deployment, open [MonkeyCAM.org](https://www.monkeycam.org), submit a
representative job, and verify the overview and downloadable G-Code.

The production worker URL is set in `src/CommonComponents.js`. If the Cloud Run
service hostname changes, update that value and rebuild the client before
deploying.
