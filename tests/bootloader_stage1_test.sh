#!/bin/bash

TEST_INJECT_WORD="__TEST_INJECT_BT1__"
TEST_MAGIC_WANT="9A11C824"

source tests/lib.sh

os_test_up "${TEST_MAGIC_WANT:?}" "${TEST_INJECT_WORD:?}" || exit -1

# Test
set -e
set -x

test_screen_content $LINENO "Bootloader: Stage 1"
test_screen_content $LINENO "Stage 2 Loaded: `build_8hexbyte bootloader/stage2`"

wait ${QEMU_PID:?}
echo "$0 passed!!!"