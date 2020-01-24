#!/bin/bash
set -e
function raise {
        echo "$1" > /dev/stderr
        exit 1
}
