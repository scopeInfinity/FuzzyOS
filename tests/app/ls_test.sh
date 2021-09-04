#!/bin/bash

# Test is supposed to be sourced from shell_test.sh

QEMU_SCREENSHOT_NAME="ls_test.ppm"

python3 -m tests.qemu.monitor -p ${MONITOR_PORT:?} -sc "ls -l"

test_create_screen_dump
test_screen_content $LINENO "r-x.*cat"
test_screen_content $LINENO "r-x.*ls"
test_screen_content $LINENO "r-x.*calc"
test_screen_content $LINENO "r-x.*tictactoe"
test_screen_content $LINENO "r--.*README.md"
