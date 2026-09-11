#!/usr/bin/env bash
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${BUILD_DIR:-${REPO_ROOT}/engine/build-dev}"
BUILD_JOBS="${BUILD_JOBS:-4}"

cmake -S "${REPO_ROOT}/engine" -B "${BUILD_DIR}" \
  -G "Unix Makefiles" \
  -DCMAKE_BUILD_TYPE=Debug

cmake --build "${BUILD_DIR}" --config Debug --parallel "${BUILD_JOBS}"

ctest --test-dir "${BUILD_DIR}" --output-on-failure

echo
echo "MonkeyCAM: ${BUILD_DIR}/cli/MonkeyCAM"
