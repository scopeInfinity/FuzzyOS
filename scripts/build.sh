#!/bin/bash
set -e
source common.sh

SOURCE=${1?"path to source.asm is missing: \$1"}
BINARY=${2?"path for output binary is missing: \$2"}
MAX_SIZE=446

echo "[Build] Starting..."
nasm -f bin "${SOURCE:?}" -o "${BINARY:?}"
binary_size="$(stat -c '%s' ${BINARY:?})"
(( ${binary_size:?} <= ${MAX_SIZE:?})) || raise "Binary size exceeds ${MAX_SIZE:?} bytes, got ${binary_size:?}"
echo "[Build] Done."