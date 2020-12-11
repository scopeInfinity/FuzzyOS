#!/bin/bash
set -e

# Preprocess
bash tests/preprocess.sh

# Execute Tests
bash tests/stage1_test.sh

# Done
echo "All tests passed!"