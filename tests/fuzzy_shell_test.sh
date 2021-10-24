#!/bin/bash

source tests/lib.sh

TEST_INJECT_WORD="${MAGIC_WORD_NO_TEST_INJECT:?}"
TEST_MAGIC_WANT="${MAGIC_WORD_SLEEP:?}"

os_test_up "${TEST_MAGIC_WANT:?}" "${TEST_INJECT_WORD:?}" || exit -1

source tests/app/shell_test.sh

python -m tests.qemu.monitor --quit
wait ${QEMU_PID:?}
echo "$0 passed!!!"