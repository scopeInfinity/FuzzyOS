#!/bin/bash

# Test is supposed to be sourced from shell_test.sh

function add_library_unit_test {
    QEMU_SCREENSHOT_NAME="usr_lib_${1:?}.ppm"

    python3 -m tests.qemu.monitor -p ${MONITOR_PORT:?} -sc ${1:?}

    test_create_screen_dump
    test_screen_content $LINENO "\[.*/usr/local/src/${1:?}.cpp\] TEST_FAILURE_COUNT: 0"
}

add_library_unit_test "test-string"
add_library_unit_test "test-stdlib"
