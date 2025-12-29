# Multi-stage image to build the MonkeyCAM engine for Linux
# Usage:
#  docker build --platform=linux/amd64 -t monkeycam-engine-builder -f engine/Dockerfile .
#  docker create --name monkeycam-build monkeycam-engine-builder
#  docker cp monkeycam-build:/opt/monkeycam ./monkeycam-dist
#  docker rm monkeycam-build
# The resulting ./monkeycam-dist directory will contain the MonkeyCAM
# binary under bin/ along with supporting artifacts.

FROM ubuntu:22.04 AS builder
ENV DEBIAN_FRONTEND=noninteractive

ARG GIT_BRANCH
ARG GIT_COMMIT_HASH
ARG GIT_COMMIT_DATE

# Install build dependencies
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

# Copy only the engine source and top-level metadata needed for install
COPY engine/ /src/engine/
COPY example/ /src/example/
COPY LICENSE README.md /src/

# Configure and build the engine
RUN cmake -S engine -B build \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX=/opt/monkeycam \
    -DGIT_BRANCH="${GIT_BRANCH}" \
    -DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}" \
    -DGIT_COMMIT_DATE="${GIT_COMMIT_DATE}"
RUN cmake --build build --config Debug
RUN g++ --version

# Install the built artifacts into a predictable location
RUN cmake --install build

# Runtime image with just the installed artifacts
FROM ubuntu:22.04 AS runtime

# Add a non-root user to make copying artifacts friendlier in some environments
RUN useradd --system --create-home monkeycam

COPY --from=builder /opt/monkeycam /opt/monkeycam

ENV PATH="/opt/monkeycam/bin:${PATH}"
WORKDIR /workspace

ENTRYPOINT ["/opt/monkeycam/bin/MonkeyCAM"]
CMD ["--help"]
