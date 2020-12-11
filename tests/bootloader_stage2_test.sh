#!/bin/bash

TEST_INJECT_WORD="__TEST_INJECT_BT2__"
TEST_MAGIC_WANT="198A65C3"

source tests/lib.sh

os_test_up "${TEST_INJECT_WORD:?}" || exit -1

# Test
set -e
echo "${SCREEN_CONTENT:?}" | grep "Bootloader: Stage 2" || \
    ( echo "Test Failed!" && exit -1 )
echo "${SCREEN_CONTENT:?}" | grep "Loading GDT Table and entering protected mode" || \
    ( echo "Test Failed!" && exit -1 )

wait ${QEMU_PID:?}
echo "$0 passed!!!"