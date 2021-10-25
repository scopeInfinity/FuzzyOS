#!/bin/bash

set -e

SRC_DIR="src/"
SRC_TEST_DIR="src_test/"
BUILD_TEST_DIR="build_test"
MONITOR_PORT=55555
QEMU_SCREENSHOT_BASEDIR="/tmp"
QEMU_SCREENSHOT_NAME="$(basename $0 .sh).ppm"  # subtests are expected to override

MAGIC_WORD_SLEEP="##SLEEP-10s##"
MAGIC_WORD_NO_TEST_INJECT="TEST-INJECT-WORD-NOT-NEEDED"

##########################################
# Raise error.
# Arguments:
#   line number
#   error message
# Output:
#   error to stderr
##########################################
function err() {
    echo -e "\e[01;31m[`basename $0`:$1] error\e[0m: $2" >&1
    exit -1
}

##########################################
# Resolves filename from build target.
# Arguments:
#   build target
# Globals:
#   BUILD_TEST_DIR
# Output:
#   filename
##########################################
function build_filename() {
    echo "${BUILD_TEST_DIR:?}/${1}"
}

##########################################
# Prints first 8 hex bytes of given file.
# Arguments:
#   build target
# Output:
#   first 8 byte
##########################################
function build_8hexbyte() {
    xxd -p -g1 -l 8 -u "`build_filename $1`"
}

##########################################
# Test Helper: Check if content is present
# on screen.
# Arguments:
#   line number
#   content
# Globals:
#   test_screen_content
# Output:
#   raises error or nothing.
##########################################
function test_screen_content() {
    echo "${SCREEN_CONTENT:?}" | grep "${2:?}" || \
    err "${1:?}" "Test Failed! Couldn't find '${2:?}' on screen."
}

##########################################
# Test Helper: Create QEMU screen dump
# Globals:
#   QEMU_SCREENSHOT_NAME
#   SCREEN_CONTENT
# Output:
#   raises error or screen dump in text.
##########################################
function test_create_screen_dump() {
    qemu_screenshot_fname="${QEMU_SCREENSHOT_BASEDIR:?}/${QEMU_SCREENSHOT_NAME:?}"
    qemu_screenshot_png="${qemu_screenshot_fname%.ppm}.png"  # lazy compute

    # Cleanup from any old screenshot
    rm -f ${qemu_screenshot_fname:?}

    python -m tests.qemu.monitor -qc screendump ${qemu_screenshot_fname:?}

    if [ ! -f ${qemu_screenshot_fname:?} ]; then
        err -1 "[create_screen_dump] failed to create qemu screen dump"
    fi

    convert ${qemu_screenshot_fname:?} ${qemu_screenshot_png:?}

    SCREEN_CONTENT="$(gocr -i ${qemu_screenshot_fname:?})"
    echo "Screen Content: '${SCREEN_CONTENT:?}'"
}

##########################################
# Activate Code for testing within source.
# Arguments:
#   Filename
#   Inject Keyword
##########################################
function inject_test_code_asm() {
    echo "Injecting test code in $1 as asm (if any)"
    sed -i "s/;\s*${2}:\s*//g" "$1"
}
export -f inject_test_code_asm

function inject_test_code_c() {
    echo "Injecting test code in $1 as c (if any)"
    sed -i "s|//\s*${2}:\s*||g" "$1"
}
export -f inject_test_code_c

##########################################
# Copy SRC to Test Sorce and Inject Code.
# Arguments:
#   Inject Keyword
##########################################
function sync_to_src_test() {
    # Prepare source code directory for tests.
    rm -r "${SRC_TEST_DIR:?}"
    cp -r "${SRC_DIR:?}" "${SRC_TEST_DIR:?}"

    if [[ "${1:?}" == "${MAGIC_WORD_NO_TEST_INJECT:?}" ]]; then
        return
    fi

    find "${SRC_TEST_DIR:?}" -iname '*.asm' -exec bash -c 'inject_test_code_asm "$0" "$1"' {} "$1" \;
    find "${SRC_TEST_DIR:?}" -iname '*.c' -exec bash -c 'inject_test_code_c "$0" "$1"' {} "$1" \;
}

###################################################
# Turn up OS in QEMU and wait for Magic Word
# Globals:
#   COMMAND_OUTPUT
#   SCREEN_CONTENT
#   QEMU_PID
# Arguments:
#   Magic Word
#   Inject Keyword
# Outputs:
#   Logs
# Returns:
#   0 on success, non-zero on error.
###################################################
function os_test_up() {
    magic_word="$1"
    test_inject_keyword="$2"

    sync_to_src_test "${test_inject_keyword:?}"

    # Turn up QEMU in background
    # Start OS with terminal
    make clean BUILD_DIR="${BUILD_TEST_DIR:?}" \
        && make qemu \
        INIT_APPNAME=sh \
        SRC_DIR="${SRC_TEST_DIR:?}" \
        BUILD_DIR="${BUILD_TEST_DIR:?}" \
        QEMU_SHUT_FLAGS="" \
        QEMU_EXTRA_FLAGS="-nographic -monitor telnet:127.0.0.1:${MONITOR_PORT:?},server,nowait" &
    QEMU_PID=$!

    while ! nc -z localhost ${MONITOR_PORT:?}; do
        echo "Waiting QEMU monitor to be available..."
        sleep 1s
    done
    echo "QEMU monitor is available."

    # Keep polling QEMU monitor until we get our magic word!
    COMMAND_OUTPUT=""
    echo "Sending commands to QEMU and polling for the magic word '${magic_word:?}' every second."

    if [[ "${magic_word:?}" == "${MAGIC_WORD_SLEEP}" ]]; then
        sleep 10s
    else
        while true; do
            COMMAND_OUTPUT="$(./tests/qemu_monitor_expect.sh ${MONITOR_PORT:?} "print \$eax")"
            echo "QEMU monitor response: '${COMMAND_OUTPUT:?}'"
            echo "${COMMAND_OUTPUT:?}" | \
                grep -i "${magic_word:?}" && \
                echo "Magic Word Found! Continuing the test..." && \
                break
            sleep 1s
        done
    fi

    sleep 1s

    test_create_screen_dump

    echo "os_up done."
    return 0
}
