#!/bin/bash

SRC_DIR="src/"
SRC_TEST_DIR="src_test/"
BUILD_TEST_DIR="build_test"
MONITOR_PORT=55555
QEMU_SCREENSHOT="/tmp/$(basename $0 .sh).ppm"
QEMU_SCREENSHOT_ARTIFACT="${QEMU_SCREENSHOT%.ppm}.png"
INIT_APPNAME="tictactoe.out"

MAGIC_WORD_SLEEP="##SLEEP-10s##"

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

    find "${SRC_TEST_DIR:?}" -iname '*.asm' -exec bash -c 'inject_test_code_asm "$0" "$1"' {} "$1" \;
    find "${SRC_TEST_DIR:?}" -iname '*.c' -exec bash -c 'inject_test_code_c "$0" "$1"' {} "$1" \;
}

###################################################
# Turn up OS in QEMU and wait for Magic Word
# Globals:
#   COMMAND_OUTPUT
#   SCREEN_CONTENT
#   QEMU_PID
#   INIT_APPNAME
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
    make clean BUILD_DIR="${BUILD_TEST_DIR:?}" \
        && make configure \
        && make qemu \
        INIT_APPNAME="${INIT_APPNAME:?}" \
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

    # Cleanup from any previous execution
    rm -f ${QEMU_SCREENSHOT:?}

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
    ./tests/qemu_monitor_expect.sh ${MONITOR_PORT:?} "screendump ${QEMU_SCREENSHOT:?}"
    ./tests/qemu_monitor_expect.sh ${MONITOR_PORT:?} "quit"

    if [ ! -f ${QEMU_SCREENSHOT:?} ]; then
        echo "Magic word found but no screenshot found! :( " >&2
        return -1
    fi

    convert ${QEMU_SCREENSHOT:?} ${QEMU_SCREENSHOT_ARTIFACT:?}

    SCREEN_CONTENT="$(gocr -i ${QEMU_SCREENSHOT:?})"
    echo "Screen Content: '${SCREEN_CONTENT:?}'"

    echo "os_up done."
    return 0
}
