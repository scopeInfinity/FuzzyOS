#!/bin/bash

SRC_DIR="src/"
SRC_TEST_DIR="src_test/"
MONITOR_PORT=55555
QEMU_SCREENSHOT="/tmp/qemu.ppm"

###################################################
# Turn up OS in QEMU and wait for $TEST_MAGIC_WANT.
# Globals:
#   TEST_MAGIC_WANT
#   COMMAND_OUTPUT
#   SCREEN_CONTENT
#   QEMU_PID
# Outputs:
#   Logs
# Returns:
#   0 on success, non-zero on error.
###################################################
function os_up() {
    # Turn up QEMU in background
    make qemu "SRC_DIR=${SRC_TEST_DIR:?}" QEMU_SHUT_FLAGS="" QEMU_EXTRA_FLAGS=" -monitor telnet:127.0.0.1:${MONITOR_PORT:?},server,nowait" &
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
    while true; do
        echo "Sending commands to QEMU."
        COMMAND_OUTPUT="$(echo -e 'screendump '${QEMU_SCREENSHOT:?}'\nprint $eax\nquit' | nc 127.0.0.1 ${MONITOR_PORT:?})"
        echo "$COMMAND_OUTPUT" | grep -i "$TEST_MAGIC_WANT" && echo "Magic Word Found! Continuing the test..." && break
        echo "Magic word not found! Got: $COMMAND_OUTPUT"
    done

    if [ ! -f ${QEMU_SCREENSHOT:?} ]; then
        echo "Magic word found but no screenshot found! :( " >&2
        return -1
    fi

    SCREEN_CONTENT="$(gocr -i ${QEMU_SCREENSHOT:?})"
    echo "Screen Content: '${SCREEN_CONTENT:?}'"

    echo "os_up done."
    return 0
}
