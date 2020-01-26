#!/bin/bash
set -e

# TODO: Fix dirs
SCRIPTS_DIR="/home/scopeinfinity/OS/scripts"
SOURCE_DIR="/home/scopeinfinity/OS/bootloader"
BUILD_DIR="/home/scopeinfinity/OS/build"

BINARY="${BUILD_DIR:?}/init"
DISK="$(realpath ../disk.vmdk)"

mkdir -p ${BUILD_DIR:?}

function gethash {
    local hash=$(md5sum "$1" 2> /dev/null | cut -d ' ' -f1 )
    echo "$hash"
}
init_lm_s=$(gethash "${BINARY}")
bash "${SCRIPTS_DIR}/build.sh" "${SOURCE_DIR}" "${BUILD_DIR}"
init_lm_d=$(gethash "${BINARY}")
if [[ "$1" = "--alsoburn" ]]; then
    if [[ "$init_lm_s" != "$init_lm_d" ]]; then
        bash "${SCRIPTS_DIR}/burn.sh" "${BINARY}" "${DISK}"
    else
        echo "Binary unchanged, ignoring burn."
    fi
fi
echo "Run Completed!"