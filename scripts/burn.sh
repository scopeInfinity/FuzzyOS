#!/bin/bash
set -e
source common.sh

BINARY=${1?"path to binary is missing: \$1"}
DISK=${2?"path for disk to burn is missing: \$2"}

echo "[Burn] Starting..."
dd bs=512 if=${BINARY:?} of=${DISK:?}
echo -ne "\x55\xaa" | dd seek=510 bs=1 of=${DISK:?}
echo "[Burn] Done."
