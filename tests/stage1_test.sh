#!/bin/bash

TEST_MAGIC_WANT="9A11C824"

source tests/lib.sh

os_up || exit -1

# Test
set -e
echo "${SCREEN_CONTENT:?}" | grep "Bootloader: Stage 1" || ( echo "Test Failed!" && exit -1 )
wait ${QEMU_PID:?}
echo "$0 passed!!!"