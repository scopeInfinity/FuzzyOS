#!/bin/bash

# Test is supposed to be sourced from shell_test.sh

QEMU_SCREENSHOT_NAME="multiprocessing_test.ppm"

python3 -m tests.qemu.monitor -p ${MONITOR_PORT:?} -sc "multiprocessing"

test_create_screen_dump
test_screen_content $LINENO "child process: says.*"
test_screen_content $LINENO "parent process: says.*"
test_screen_content $LINENO "Is current process the child:.*Yes"
test_screen_content $LINENO "Is current process the child:.*No"
