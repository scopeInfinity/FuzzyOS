#!/bin/bash

# Test is supposed to be sourced from fuzzy_app_test.sh

test_screen_content $LINENO "Simple Shell"

# Test applications
source tests/app/ls_test.sh
source tests/app/tictactoe_test.sh
source tests/app/cat_test.sh
source tests/app/multiprocessing_test.sh
source tests/app/libraries/usr_lib_test.sh
