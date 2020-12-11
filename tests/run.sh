#!/bin/bash
set -e

# Execute Tests
bash tests/bootloader_stage1_test.sh
bash tests/bootloader_stage2_test.sh

# Done
echo "All tests passed!"