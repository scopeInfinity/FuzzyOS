# FuzzyOS [![CI](https://github.com/scopeInfinity/FuzzyOS/actions/workflows/ci.yaml/badge.svg)](https://github.com/scopeInfinity/FuzzyOS/actions/workflows/ci.yaml) [![Hits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2FscopeInfinity%2FFuzzyOS&count_bg=%2379C83D&title_bg=%23555555&icon=&icon_color=%23E7E7E7&title=hits&edge_flat=false)](https://hits.seeyoufarm.com)
Experimental OS

- The project is still under development:exclamation:

### Release

Please find all releases at https://github.com/scopeInfinity/FuzzyOS/releases

The VMDK image can be found under `Assets` for the corresponding release.


### Screenshots

The screenshots can be located as `Artifacts` under completed run on [Actions/CI](https://github.com/scopeInfinity/FuzzyOS/actions/workflows/ci.yaml?query=branch%3Amaster+event%3Apush+is%3Asuccess).


 Bootloader  |  Kernel Turnup
-------------|----------------
 ![image](https://user-images.githubusercontent.com/9819066/119272271-12b67700-bbfd-11eb-8036-1466d39ebe8e.png) | ![image](https://user-images.githubusercontent.com/9819066/129453488-1950ca70-25cc-4801-842f-b25ea88ab25f.png)


 Simple Shell | TicTacToe Game
--------------| ------------------
![image](https://user-images.githubusercontent.com/9819066/129463788-94686a08-236e-49a9-b81b-6325e4b2ab58.png) | ![image](https://user-images.githubusercontent.com/9819066/129463802-d9a0bc77-74eb-4438-b553-e1439ada95a1.png)

### Boot OS

#### How to get boot image?
  - Download image from one of the [Release](#Release).
  - Or directly build image using `make images` after cloning the repository.

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
make clean
```

#### QEMU Quick Launch

```
make qemu
```

#### Build

Use make to build binaries and image files.

```
make
```

##### Tests

At this moment tests works by injecting predefined ASM in the source code which halts
the execution at the point of interest. After that tests can make use of QEMU monitor
to verify the current state with expected state.

```
make test
```

##### Debug

Execute QEMU in debug mode and setup GDB server.

```
make qemu_debug
```

And then connect to GDB Server.

```
make qemu_debug_connect
```

