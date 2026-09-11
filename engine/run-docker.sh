#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage:
  engine/run-docker.sh \
    --board PATH \
    [--binding PATH] \
    --machine PATH \
    --outdir DIRECTORY \
    [--bindingdist CENTIMETERS]

The input paths may be anywhere Docker Desktop can read. The output directory
is created if necessary. The engine image is rebuilt unless SKIP_BUILD=1.

Environment:
  IMAGE_TAG    Docker image to build/run (default: monkeycam-engine-builder)
  PLATFORM     Docker platform (default: linux/amd64)
  SKIP_BUILD   Set to 1 to reuse an existing image
EOF
}

absolute_file() {
  local value="$1"
  local directory
  directory="$(cd "$(dirname "${value}")" && pwd -P)"
  printf '%s/%s\n' "${directory}" "$(basename "${value}")"
}

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
IMAGE_TAG="${IMAGE_TAG:-monkeycam-engine-builder}"
PLATFORM="${PLATFORM:-linux/amd64}"
SKIP_BUILD="${SKIP_BUILD:-0}"

BOARD_PATH=""
BINDING_PATH=""
MACHINE_PATH=""
OUTPUT_DIR=""
BINDING_DISTANCE=""

while [[ $# -gt 0 ]]; do
  case "$1" in
    --board)
      BOARD_PATH="${2:-}"
      shift 2
      ;;
    --binding)
      BINDING_PATH="${2:-}"
      shift 2
      ;;
    --machine)
      MACHINE_PATH="${2:-}"
      shift 2
      ;;
    --outdir)
      OUTPUT_DIR="${2:-}"
      shift 2
      ;;
    --bindingdist)
      BINDING_DISTANCE="${2:-}"
      shift 2
      ;;
    -h|--help)
      usage
      exit 0
      ;;
    *)
      echo "Unknown argument: $1" >&2
      usage >&2
      exit 2
      ;;
  esac
done

if [[ -z "${BOARD_PATH}" || -z "${MACHINE_PATH}" || -z "${OUTPUT_DIR}" ]]; then
  usage >&2
  exit 2
fi

for input_path in "${BOARD_PATH}" "${MACHINE_PATH}"; do
  if [[ ! -f "${input_path}" ]]; then
    echo "Input file does not exist: ${input_path}" >&2
    exit 2
  fi
done

BOARD_PATH="$(absolute_file "${BOARD_PATH}")"
MACHINE_PATH="$(absolute_file "${MACHINE_PATH}")"
if [[ -n "${BINDING_PATH}" ]]; then
  if [[ ! -f "${BINDING_PATH}" ]]; then
    echo "Input file does not exist: ${BINDING_PATH}" >&2
    exit 2
  fi
  BINDING_PATH="$(absolute_file "${BINDING_PATH}")"
fi
mkdir -p "${OUTPUT_DIR}"
OUTPUT_DIR="$(cd "${OUTPUT_DIR}" && pwd -P)"

if [[ "${SKIP_BUILD}" != "1" ]]; then
  IMAGE_TAG="${IMAGE_TAG}" PLATFORM="${PLATFORM}" "${REPO_ROOT}/engine/build-docker.sh"
fi

command=(
  docker run --rm
  --platform "${PLATFORM}"
  --mount "type=bind,src=${BOARD_PATH},dst=/inputs/board.json,readonly"
  --mount "type=bind,src=${MACHINE_PATH},dst=/inputs/machine.json,readonly"
  --mount "type=bind,src=${OUTPUT_DIR},dst=/output"
)

if [[ -n "${BINDING_PATH}" ]]; then
  command+=(--mount "type=bind,src=${BINDING_PATH},dst=/inputs/binding.json,readonly")
fi

command+=(
  "${IMAGE_TAG}"
  --board /inputs/board.json
  --machine /inputs/machine.json
  --outdir /output
)

if [[ -n "${BINDING_PATH}" ]]; then
  command+=(--binding /inputs/binding.json)
fi

if [[ -n "${BINDING_DISTANCE}" ]]; then
  command+=(--bindingdist "${BINDING_DISTANCE}")
fi

"${command[@]}"

echo
echo "Output: ${OUTPUT_DIR}"
