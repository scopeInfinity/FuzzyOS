#!/bin/bash

# Test is supposed to be sourced from shell_test.sh

QEMU_SCREENSHOT_NAME="ls_test.ppm"

python3 -m tests.qemu.monitor -p ${MONITOR_PORT:?} -sc ls

test_create_screen_dump
test_screen_content $LINENO "cat"
test_screen_content $LINENO "calc"
test_screen_content $LINENO "ls"
test_screen_content $LINENO "tictactoe"
