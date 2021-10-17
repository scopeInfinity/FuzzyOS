#!/bin/bash

# Test is supposed to be sourced from shell_test.sh

QEMU_SCREENSHOT_NAME="usr_lib_string_test.ppm"

python3 -m tests.qemu.monitor -p ${MONITOR_PORT:?} -sc test-string

test_create_screen_dump
test_screen_content $LINENO "TEST_FAILURE_COUNT: 0"
