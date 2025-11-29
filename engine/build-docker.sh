#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
IMAGE_TAG=${IMAGE_TAG:-monkeycam-engine-builder}
PLATFORM=${PLATFORM:-linux/amd64}

GIT_BRANCH=$(git -C "${ROOT_DIR}" rev-parse --abbrev-ref HEAD)
GIT_COMMIT_HASH=$(git -C "${ROOT_DIR}" log -1 --format=%h)
GIT_COMMIT_DATE=$(git -C "${ROOT_DIR}" log -1 --format=%cD)

docker build \
  --platform="${PLATFORM}" \
  -t "${IMAGE_TAG}" \
  -f "${ROOT_DIR}/engine/Dockerfile" \
  --build-arg GIT_BRANCH="${GIT_BRANCH}" \
  --build-arg GIT_COMMIT_HASH="${GIT_COMMIT_HASH}" \
  --build-arg GIT_COMMIT_DATE="${GIT_COMMIT_DATE}" \
  "${ROOT_DIR}"
