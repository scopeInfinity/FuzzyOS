#!/bin/bash
set -e

source tests/lib.sh

##########################################
# Activate Code for testing within source.
# Arguments:
#   Filename
##########################################
function inject_test_code() {
    echo "Injecting Test Code in $1 (if any)"
    sed -i 's/;\s*__TEST_INJECT__:\s*//g' "$1"
}
export -f inject_test_code

# Prepare source code directory for tests.
rsync -r "${SRC_DIR:?}" "${SRC_TEST_DIR:?}"

find "${SRC_TEST_DIR:?}" -iname '*.asm' -exec bash -c 'inject_test_code "$0"' {} \;

