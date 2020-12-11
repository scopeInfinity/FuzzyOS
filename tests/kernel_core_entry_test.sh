#!/bin/bash

TEST_INJECT_WORD="__TEST_INJECT_KERNEL_CORE_ENTRY__"
TEST_MAGIC_WANT="922E52FF"

source tests/lib.sh

os_test_up "${TEST_INJECT_WORD:?}" || exit -1

# Test
set -e
echo "${SCREEN_CONTENT:?}" | grep "Initializing Kernel" || \
    ( echo "Test Failed!" && exit -1 )
echo "${SCREEN_CONTENT:?}" | grep "Loading IDT Table" || \
    ( echo "Test Failed!" && exit -1 )

wait ${QEMU_PID:?}
echo "$0 passed!!!"