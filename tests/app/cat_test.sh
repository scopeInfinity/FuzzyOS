#!/bin/bash

# Test is supposed to be sourced from shell_test.sh

QEMU_SCREENSHOT_NAME="cat_test.ppm"

python3 -m tests.qemu.monitor -p ${MONITOR_PORT:?} -sc cat readme.md

test_create_screen_dump
test_screen_content $LINENO "FuzzyOS"
test_screen_content $LINENO "32-bit x86"
