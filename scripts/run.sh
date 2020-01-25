#!/bin/bash
set -e

# TODO: Fix dirs
SCRIPTS_DIR="/home/scopeinfinity/OS/scripts"
BUILD_DIR="/home/scopeinfinity/OS/build"

SOURCE="$(realpath main.asm)"
BINARY="${BUILD_DIR:?}/main"
DISK="$(realpath ../disk.vmdk)"

mkdir -p ${BUILD_DIR:?}

bash "${SCRIPTS_DIR}/build.sh" "${SOURCE}" "${BINARY}"
bash "${SCRIPTS_DIR}/burn.sh" "${BINARY}" "${DISK}"
echo "Run Completed!"