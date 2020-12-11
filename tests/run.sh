#!/bin/bash
set -e

# Execute Tests
bash tests/stage1_test.sh

# Done
echo "All tests passed!"