#!/usr/bin/env bash
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
IMAGE_TAG="${IMAGE_TAG:-monkeycam-engine-builder}"
PLATFORM="${PLATFORM:-linux/amd64}"
TARGET="${REPO_ROOT}/webapp/worker/bin/linux/MonkeyCAM"

IMAGE_TAG="${IMAGE_TAG}" PLATFORM="${PLATFORM}" "${REPO_ROOT}/engine/build-docker.sh"

CONTAINER_ID="$(docker create --platform "${PLATFORM}" "${IMAGE_TAG}")"
cleanup() {
  docker rm "${CONTAINER_ID}" >/dev/null
}
trap cleanup EXIT

docker cp "${CONTAINER_ID}:/opt/monkeycam/bin/MonkeyCAM" "${TARGET}"
chmod 0755 "${TARGET}"

echo "Staged Linux engine at ${TARGET}"
