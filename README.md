# FuzzyOS [![Build Status](https://travis-ci.com/scopeInfinity/FuzzyOS.svg?branch=master)](https://travis-ci.com/scopeInfinity/FuzzyOS)
Experimental OS

- Not yet ready.

#### Prerequisite

```
bash before_install.sh
```

#### Usage
- Use make to build binaries and image files.
  - `make`

##### Boot image on a VM
###### QEMU
  - `make qemu`

###### VMware
  - Create *Virtual Machine Disk* with fixed size of *4MB*.
  - Burn `build/image.vmdk` to `new-disk-flat.vmdk`
    - `bash scripts/burn.sh build/image.vmdk /path/to/build/new-disk-flat.vmdk`
  - Or replace `new-disk-flat.vmdk` with a sym-link to `build/image.vmdk` :grin:

##### Boot image on a real machine
  - Use `dd` or `scripts/burn.sh` to burn image into the disk (potentially destructive).
  - ```
    bash scripts/burn.sh build/image.vmdk /path/to/devicefile
    ```
###### Issues on booting from the Flash Drive
- Try formatting device MBR with a FAT partition.
- And then burn it again with test image.

#### Development

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
