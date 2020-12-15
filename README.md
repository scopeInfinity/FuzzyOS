# FuzzyOS [![Build](https://github.com/scopeInfinity/FuzzyOS/workflows/Build/badge.svg)](https://github.com/scopeInfinity/FuzzyOS/actions?query=workflow%3ABuild) [![Test](https://github.com/scopeInfinity/FuzzyOS/workflows/Test/badge.svg)](https://github.com/scopeInfinity/FuzzyOS/actions?query=workflow%3ATest) 
Experimental OS

- The project is still under development:exclamation:

### Release

Please find all releases at https://github.com/scopeInfinity/FuzzyOS/releases

The new releases should have following artifacts:
 - VMDK image
 - Test Screenshots
 - Source code

### Boot OS

#### How to get boot image?
  - Download image from one of the [Release](#Release).
  - Or directly build image using `make qemu` after cloning the repository.

#### Boot on VMware
  - Create *Virtual Machine Disk* with fixed size of *4MB*.
  - Burn `image.vmdk` to `new-disk-flat.vmdk`
    - `bash scripts/burn.sh /path/to/image.vmdk /path/to/build/new-disk-flat.vmdk`
  - Or simply replace `new-disk-flat.vmdk` with a sym-link to `build/image.vmdk` (for rapid development :grin:)

##### Boot on a real machine
  - Use `dd` or `scripts/burn.sh` to burn image into the disk (potentially destructive).
  - `bash scripts/burn.sh build/image.vmdk /path/to/devicefile`
 
###### If boot from Flash Drive doesn't work
  - Try formatting device MBR with a FAT partition.
  - And then burn the image again.


### Development

#### Prerequisite

```
bash before_install.sh
```

#### QEMU Quick Launch

`make qemu`

#### Build

Use make to build binaries and image files.
  - `make`

##### Tests

At this moment tests works by injecting predefined ASM in the source code which halts
the execution at the point of interest. After that tests can make use of QEMU monitor
to verify the current state with expected state.

 - `make test`

##### Debug

Execute QEMU in debug mode and setup GDB server.
 - `make qemu_debug`

And then connect to GDB Server.
 - `make qemu_debug_connect`
