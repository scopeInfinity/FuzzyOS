#!/bin/bash
set -e
source "$(dirname $0)/common.sh"

SOURCE=${1?"path to source.asm is missing: \$1"}
SOURCE_DIR="$(dirname $SOURCE)"
BINARY=${2?"path for output binary is missing: \$2"}
MAX_SIZE=446

echo "[Build] Starting..."
nasm -f bin "${SOURCE:?}" -o "${BINARY:?}" -i "${SOURCE_DIR}" 
binary_size="$(stat -c '%s' ${BINARY:?})"
(( ${binary_size:?} <= ${MAX_SIZE:?})) || raise "Binary size exceeds ${MAX_SIZE:?} bytes, got ${binary_size:?}"
echo "[Build] done. BinarySize: ${binary_size} bytes. MaxSize: ${MAX_SIZE} bytes."