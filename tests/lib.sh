#!/bin/bash

SRC_DIR="src/"
SRC_TEST_DIR="src_test/"
BUILD_TEST_DIR="build_test"
MONITOR_PORT=55555
QEMU_SCREENSHOT="/tmp/qemu.ppm"


##########################################
# Activate Code for testing within source.
# Arguments:
#   Filename
#   Inject Keyword
##########################################
function inject_test_code() {
    echo "Injecting Test Code in $1 (if any)"
    sed -i "s/;\s*${2}:\s*//g" "$1"
}
export -f inject_test_code


##########################################
# Copy SRC to Test Sorce and Inject Code.
# Arguments:
#   Inject Keyword
##########################################
function sync_to_src_test() {
    # Prepare source code directory for tests.
    rsync -r "${SRC_DIR:?}" "${SRC_TEST_DIR:?}"

    find "${SRC_TEST_DIR:?}" -iname '*.asm' -exec bash -c 'inject_test_code "$0" "$1"' {} "$1" \;
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
    make qemu \
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

    while true; do
        COMMAND_OUTPUT="$(./tests/qemu_monitor_expect.sh ${MONITOR_PORT:?} "print \$eax")"
        echo "QEMU monitor response: '${COMMAND_OUTPUT:?}'"
        echo "${COMMAND_OUTPUT:?}" | \
            grep -i "${magic_word:?}" && \
            echo "Magic Word Found! Continuing the test..." && \
            break
        sleep 1s
    done

    sleep 5s
    ./tests/qemu_monitor_expect.sh ${MONITOR_PORT:?} "screendump ${QEMU_SCREENSHOT:?}"
    ./tests/qemu_monitor_expect.sh ${MONITOR_PORT:?} "quit"

    if [ ! -f ${QEMU_SCREENSHOT:?} ]; then
        echo "Magic word found but no screenshot found! :( " >&2
        return -1
    fi

    SCREEN_CONTENT="$(gocr -i ${QEMU_SCREENSHOT:?})"
    echo "Screen Content: '${SCREEN_CONTENT:?}'"

    echo "os_up done."
    return 0
}