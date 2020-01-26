#!/bin/bash
set -e
source "$(dirname $0)/common.sh"

SOURCE_DIR=${1?"path to source dir is missing: \$1"}
BUILD_DIR=${2?"path to build dir is missing: \$2"}
MAX_SIZE=446

echo "[Build] Starting..."
nasm "${SOURCE_DIR:?}/main_low.asm" -o "${BUILD_DIR:?}/main_low.o" -f elf64
gcc -c "${SOURCE_DIR:?}/main.c" -o "${BUILD_DIR:?}/main.o"
nasm -f bin "${SOURCE_DIR:?}/init.asm" -o "${BUILD_DIR:?}/init" -i "${SOURCE_DIR}" 

binary_size="$(stat -c '%s' ${BUILD_DIR:?}/init )"
(( ${binary_size:?} <= ${MAX_SIZE:?})) || raise "Binary size exceeds ${MAX_SIZE:?} bytes, got ${binary_size:?}"
echo "[Build] done. BinarySize: ${binary_size} bytes. MaxSize: ${MAX_SIZE} bytes."