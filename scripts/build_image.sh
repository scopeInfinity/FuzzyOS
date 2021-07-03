#!/bin/bash
# The scripts take multiple subimages as argument and prepare final image
# by merging them. Once successful echo's sub images size.
set -e
if [ $# -lt 2 ] ; then
    echo "Usage: build_image.sh <target_image> <subimage1> [ <subimage2> ... ]" >&2
    exit 1
fi

# Currently image is build in two phases.
# Phase 1:
#   Build image with undefined SECTOR index which can be used to calculate
#   SECTOR index and cache them.
# Phase 2:
#   Populate the Makefile SECTOR indexes from the cache and retrigger build
#   to generate final image.

IMAGE_FILE="$1"
shift

echo "Using $IMAGE_FILE as image file" >&2
rm "${IMAGE_FILE:?}" || echo "Can't remove existing image file, continuing..." >&2
touch "${IMAGE_FILE}"

subImagesSectorCount=( )
for target in "$@"
do
    echo "Appending '$target' to image" >&2
    fsize=$(stat -c "%s" ${target:?})
    cat ${target:?}  >> ${IMAGE_FILE:?}

    if [ "$(( $fsize % 512 ))" -ne 0 ]; then
        echo "The intermediate image $target size isn't divisible by 512" >&2
        exit 2
    fi
    subImagesSectorCount+=($(($fsize / 512)) )
done
echo "${subImagesSectorCount[@]}"
