#!/bin/bash

# Test is supposed to be sourced from shell_test.sh

QEMU_SCREENSHOT_NAME="cat_test.ppm"

python3 -m tests.qemu.monitor -p ${MONITOR_PORT:?} -sc run cat readme.md

test_create_screen_dump
test_screen_content $LINENO "Experimental OS"
test_screen_content $LINENO "The project is still under development"
