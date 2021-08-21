#!/bin/bash

source tests/lib.sh

TEST_INJECT_WORD="__TEST_INJECT_APP_TTT_ENTRY__"
TEST_MAGIC_WANT="${MAGIC_WORD_SLEEP:?}"

os_test_up "${TEST_MAGIC_WANT:?}" "${TEST_INJECT_WORD:?}" INIT_APPNAME="tictactoe" || exit -1

# Test
set -e
test_screen_content $LINENO "TicTacToe"

python -m tests.qemu.monitor --quit
wait ${QEMU_PID:?}
echo "$0 passed!!!"