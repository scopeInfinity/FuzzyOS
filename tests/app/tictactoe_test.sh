#!/bin/bash

# Test is supposed to be sourced from shell_test.sh

QEMU_SCREENSHOT_NAME="tictactoe_test.ppm"

python3 -m tests.qemu.monitor -p ${MONITOR_PORT:?} -sc tictactoe

test_create_screen_dump
test_screen_content $LINENO "TicTacToe"
test_screen_content $LINENO "Q to quit game"

python3 -m tests.qemu.monitor -p ${MONITOR_PORT:?} -sc 'q'
