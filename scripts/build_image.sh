#!/bin/bash
# The scripts take essential subimages as argument merging them while verifying their size.

set -e
if [ $# -ne 9 ] ; then
    echo "Usage: build_image.sh <target_image> <bt_stage1> <sc> <realmode_lib> <sc> <bt_stage2> <sc> <kernel> <sc>"
    echo "<sc> is sector count of preceding subimage."
    exit 1
fi

IMAGE_FILE="$1"
TMP_SUBIMAGE="/tmp/fuzzy_subimage"
shift

function verify_subimage_and_push() {
    subimage_name="${1:?}"
    subimage="${2:?}"
    min_sectors="${3:?}"
    max_sectors="${4:?}"

    echo "Verifying ${subimage_name:?}: ${subimage:?} min_sectors: ${min_sectors:?} max_sectors: ${max_sectors:?}"
    fsize=$(stat -c "%s" ${subimage:?})

    if [ "$(( $fsize % 512 ))" -ne 0 ]; then
        echo "The ${subimage} size isn't divisible by 512" >&2
        exit 2
    fi
    if [ "$(( $fsize / 512 ))" -lt ${min_sectors:?} ]; then
        echo "The ${subimage} uses less sectors than ${min_sectors:?}" >&2
        exit 2
    fi
    if [ "$(( $fsize / 512 ))" -gt ${max_sectors:?} ]; then
        echo "The ${subimage} uses more sectors than ${max_sectors:?}" >&2
        exit 2
    fi

    cp -f ${subimage:?} ${TMP_SUBIMAGE:?}
    truncate --size=%"$(( $max_sectors * 512 ))" ${TMP_SUBIMAGE:?}
    cat ${TMP_SUBIMAGE:?} >> ${IMAGE_FILE:?}
}

echo "Using $IMAGE_FILE as image file" >&2
rm "${IMAGE_FILE:?}" || echo "Can't remove existing image file, continuing..." >&2
touch "${IMAGE_FILE}"

# sum of all should not exceed 256 sectors
verify_subimage_and_push "bt_stage1"     $1  $2 $2
verify_subimage_and_push "realmode_lib"  $3  $4 $4
verify_subimage_and_push "bt_stage2"     $5  $6 $6
verify_subimage_and_push "kernel"        $7  $8 $8
