#!/bin/bash

TEST_INJECT_WORD="__TEST_INJECT_BT2__"
TEST_MAGIC_WANT="198A65C3"

source tests/lib.sh

os_test_up "${TEST_MAGIC_WANT:?}" "${TEST_INJECT_WORD:?}" || exit -1

# Test
set -e

test_screen_content $LINENO "Bootloader: Stage 2"
test_screen_content $LINENO "Static library loaded at 0x00007E00: `build_8hexbyte real_mode/static_library`"
test_screen_content $LINENO "Kernel loaded at 0x[0-9A-Fa-f]\+: `build_8hexbyte kernel/core`"
test_screen_content $LINENO "Loading GDT Table and entering protected mode"

wait ${QEMU_PID:?}
echo "$0 passed!!!"