#!/bin/bash
set -e

IMAGE=${1?"path to image is missing: \$1"}
DISK=${2?"path for disk for burning is missing: \$2"}

echo "[Burn] Starting..."
dd bs=512 if=${IMAGE:?} of=${DISK:?}
echo "[Burn] Done."
