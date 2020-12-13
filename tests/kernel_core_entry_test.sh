#!/bin/bash

TEST_INJECT_WORD="__TEST_INJECT_KERNEL_CORE_ENTRY__"
TEST_MAGIC_WANT="922E52FF"

source tests/lib.sh

os_test_up "${TEST_MAGIC_WANT:?}" "${TEST_INJECT_WORD:?}" || exit -1

# Test
set -e
test_screen_content $LINENO "Initializing Kernel"
test_screen_content $LINENO "Registering syscalls"
test_screen_content $LINENO "Loading IDT Table"


wait ${QEMU_PID:?}
echo "$0 passed!!!"