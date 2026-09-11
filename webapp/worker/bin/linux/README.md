# Linux engine staging directory

From the repository root, run:

```sh
./webapp/worker/stage-linux-engine.sh
```

The generated `MonkeyCAM` binary is ignored by Git and included in the worker
container build context. See the [worker guide](../../README.md#deploy-to-cloud-run).
