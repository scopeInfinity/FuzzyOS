#!/bin/bash
set -e

# TODO: Fix dirs
SCRIPTS_DIR="/home/scopeinfinity/OS/scripts"
BUILD_DIR="/home/scopeinfinity/OS/build"

SOURCE="$(realpath main.asm)"
BINARY="${BUILD_DIR:?}/main"
DISK="$(realpath ../disk.vmdk)"

cd "${SCRIPTS_DIR}"
bash build.sh "${SOURCE}" "${BINARY}"
bash burn.sh "${BINARY}" "${DISK}"
echo "Run Completed!"