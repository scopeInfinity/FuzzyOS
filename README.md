# FuzzyOS
Experimental OS

- Not yet ready.

#### Usage
- Use make to build binaries and image files.
  - `make`

##### Boot image on a VM
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
