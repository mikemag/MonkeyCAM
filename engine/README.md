# MonkeyCAM engine development

The engine is a C++ command-line application. It reads board, binding, and
machine JSON files and writes an overview HTML file plus the G-Code programs.

Run commands in this guide from the repository root unless noted otherwise.

## Build and test natively

The helper script configures a Debug build, builds the CLI, and runs the complete
CTest suite:

```sh
./engine/build-native.sh
```

The defaults are:

- build directory: `engine/build-dev`
- parallel build jobs: `4`
- binary: `engine/build-dev/cli/MonkeyCAM`

Override the first two when needed:

```sh
BUILD_DIR=/tmp/monkeycam-build BUILD_JOBS=8 ./engine/build-native.sh
```

The script uses Unix Makefiles and the platform compiler selected by CMake. It
does not add warning suppressions; each target retains the warning policy in its
CMake configuration.

To run tests again without rebuilding:

```sh
ctest --test-dir engine/build-dev --output-on-failure
```

To run only the end-to-end G-Code regression:

```sh
ctest --test-dir engine/build-dev \
  -R max-cut-depth-regression \
  --output-on-failure
```

The same regression can be invoked directly, which is useful when testing a
specific binary:

```sh
python3 engine/test/verify-max-cut-depth.py \
  --monkeycam engine/build-dev/cli/MonkeyCAM \
  --repo-root .
```

Python 3 must be available when CMake configures the build or the regression test
will not be registered with CTest. Check the discovered tests with:

```sh
ctest --test-dir engine/build-dev --show-only
```

## Run the native engine

Snowboard:

```sh
OUTPUT_DIR="$(mktemp -d /tmp/monkeycam-snowboard.XXXXXX)"
engine/build-dev/cli/MonkeyCAM \
  --board example/board-def.json \
  --binding example/board-binding-def.json \
  --machine example/machine-tool-def.json \
  --outdir "${OUTPUT_DIR}"
```

Ski (the CLI requires the distance between the pair of skis):

```sh
OUTPUT_DIR="$(mktemp -d /tmp/monkeycam-ski.XXXXXX)"
engine/build-dev/cli/MonkeyCAM \
  --board example/ski-def.json \
  --binding example/ski-binding-def.json \
  --bindingdist 30 \
  --machine example/machine-tool-def.json \
  --outdir "${OUTPUT_DIR}"
```

Splitboard:

```sh
OUTPUT_DIR="$(mktemp -d /tmp/monkeycam-splitboard.XXXXXX)"
engine/build-dev/cli/MonkeyCAM \
  --board example/splitboard-def.json \
  --binding example/splitboard-binding-def.json \
  --machine example/machine-tool-def.json \
  --outdir "${OUTPUT_DIR}"
```

## Build and run with Docker

The Docker build produces the Linux/AMD64 runtime used by the Cloud Run worker:

```sh
./engine/build-docker.sh
```

The default image is `monkeycam-engine-builder`. Override its tag or target
platform with `IMAGE_TAG` and `PLATFORM`:

```sh
IMAGE_TAG=monkeycam-engine-dev PLATFORM=linux/amd64 ./engine/build-docker.sh
```

`run-docker.sh` rebuilds that image and maps three arbitrary host config files
plus an output directory into it. For example, this uses the maximum-cut-depth
test fixture with a splitboard:

```sh
OUTPUT_DIR="$(mktemp -d /tmp/monkeycam-depth.XXXXXX)"
./engine/run-docker.sh \
  --board example/splitboard-def.json \
  --binding example/splitboard-binding-def.json \
  --machine engine/test/data/machine-max-cut-depth.json \
  --outdir "${OUTPUT_DIR}"
```

For several runs against the same source build, skip the rebuild after the first:

```sh
SKIP_BUILD=1 ./engine/run-docker.sh \
  --board /absolute/path/to/board.json \
  --binding /absolute/path/to/binding.json \
  --machine /absolute/path/to/machine.json \
  --outdir /absolute/path/to/output
```

Add `--bindingdist 30` for a ski. `run-docker.sh --help` lists all wrapper
options. Docker Desktop must be allowed to read any input directories outside
the repository. If another Docker context is selected, prefix the build or run
with `DOCKER_CONTEXT=desktop-linux` to use Docker Desktop without changing the
global context.

The final image also accepts the CLI arguments directly. Its installed examples
are under `/opt/monkeycam/example`, and its entry point is
`/opt/monkeycam/bin/MonkeyCAM`.

## Validate generated G-Code

The checked-in `Sample-output` directory is the compatibility baseline. The
end-to-end regression normalizes generated timestamps and engine Git metadata
before comparing output. Its scenarios cover:

- no new settings, which must preserve established G-Code;
- non-binding `max cut depth` values, which must also preserve G-Code;
- active limits on Core Top Cutout, Edge Trench, Splitboard Center Trench, and
  Core Top Profile;
- unchanged X/Y geometry, final depths, feeds, and bounding boxes while passes
  are added.

Use the regression rather than a raw directory diff for the compatibility check.
Exact floating-point formatting can differ in the last printed digit between
Apple Clang and the Linux/GCC container.

For hands-on validation, copy a machine config, change its pass settings or a
tool's `max cut depth`, run the same board into separate output directories, and
compare the `.nc` programs. The relevant configuration is documented in the
[Configuration Guide](docs/Configuration_Guide.md); program behavior is described
in the [G-Code Program Guide](docs/G-Code_Program_Guide.md).

## Package the engine for the worker

The worker image needs a Linux/AMD64 engine binary in
`webapp/worker/bin/linux/MonkeyCAM`. Build and stage it with:

```sh
./webapp/worker/stage-linux-engine.sh
```

That file is intentionally ignored by Git. Continue with the
[worker deployment guide](../webapp/worker/README.md#deploy-to-cloud-run).
