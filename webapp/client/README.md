# Running Locally
- `cd webapp/client`
- `fnm use`
- `npm install`
- `npm start`
    - Starts a dev server on port 3000
    - Hits a local worker on 8080
    - Change the `proxy` in package.json to point to other local servers, or to the real server address.

# Deploying the client
Deploying a static build to Google App Engine.

- `cd webapp/client`
- `fnm use`
- `npm install`
- `npm run build`
- `gcloud app deploy --project "monkeycam-web-app"`

