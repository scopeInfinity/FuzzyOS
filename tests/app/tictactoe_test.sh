#!/bin/bash

# Test is supposed to be sourced from shell_test.sh

python3 -m tests.qemu.monitor -p ${MONITOR_PORT:?} -sc run tictactoe

test_create_screen_dump
test_screen_content $LINENO "TicTacToe"
test_screen_content $LINENO "Q to quit game"

python3 -m tests.qemu.monitor -p ${MONITOR_PORT:?} -sc 'q'
