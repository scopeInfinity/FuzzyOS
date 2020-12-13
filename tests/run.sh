#!/bin/bash
set -e

# Execute Tests
timeout 2m bash tests/bootloader_stage1_test.sh
timeout 2m bash tests/bootloader_stage2_test.sh
timeout 2m bash tests/kernel_core_entry_test.sh

# Done
echo "All tests passed!"