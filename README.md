# FuzzyOS [![CI](https://github.com/scopeInfinity/FuzzyOS/actions/workflows/ci.yaml/badge.svg)](https://github.com/scopeInfinity/FuzzyOS/actions/workflows/ci.yaml) [![Hits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2FscopeInfinity%2FFuzzyOS&count_bg=%2379C83D&title_bg=%23555555&icon=&icon_color=%23E7E7E7&title=hits&edge_flat=false)](https://hits.seeyoufarm.com)
32-bit x86 multiprocessing OS capable of running console or graphical user applications.

### Release

Please find all releases at https://github.com/scopeInfinity/FuzzyOS/releases

The boot image can be found under `Assets` for the corresponding release.


### Screenshots

The screenshots can be located as `Artifacts` under completed run on [Actions/CI](https://github.com/scopeInfinity/FuzzyOS/actions/workflows/ci.yaml?query=branch%3Amaster+event%3Apush+is%3Asuccess).


 Bootloader  |  Kernel Turnup
-------------|----------------
 ![image](https://user-images.githubusercontent.com/9819066/119272271-12b67700-bbfd-11eb-8036-1466d39ebe8e.png) | ![image](https://user-images.githubusercontent.com/9819066/129453488-1950ca70-25cc-4801-842f-b25ea88ab25f.png)


 Simple Shell | cat
--------------| ------
![image](https://user-images.githubusercontent.com/9819066/132931063-21868d2e-4e55-4a25-843d-9a6a9d1830f6.png) |  ![image](https://user-images.githubusercontent.com/9819066/132112873-f47c8588-1e23-4c4e-9237-a9783c70defb.png)

fork()          | TicTacToe Game
----------------|---------------
![image](https://user-images.githubusercontent.com/9819066/132112808-72ba7691-5f18-4631-b95d-fa52a63d2f38.png) | ![image](https://user-images.githubusercontent.com/9819066/129463802-d9a0bc77-74eb-4438-b553-e1439ada95a1.png)

| PingPong Game |  Logo Programming |
| ------------- | ------------- |
| ![gif](https://user-images.githubusercontent.com/9819066/132988756-de1b7770-430d-40df-a7c9-593cd752b20a.gif) | ![gif](https://user-images.githubusercontent.com/9819066/134997094-c44f70cb-dca9-4260-89fb-2676fad38631.gif) |

| Desktop |
| ------- |
| ![image](https://user-images.githubusercontent.com/9819066/138595043-c8f5c6a7-30a4-448a-a6c6-ee62fe00946e.png) |



### Boot OS

#### How to get boot image?
  - Download image from one of the [Release](#Release).
  - Or directly build image using `make images` after cloning the repository.

#### Boot on VirtualBox
  - Create VM with `FuzzyOS.vdi` as storage.

#### Boot on VMware
  - Create VM with `FuzzyOS.vmdk` as storage.

##### Boot on a real machine
  - Use `dd` or `scripts/burn.sh` to burn image into the disk (potentially destructive).
  - `bash scripts/burn.sh build/FuzzyOS.raw /path/to/devicefile`


### Features

| Feature                                                     | Exists             |
|-------------------------------------------------------------|--------------------|
| Boot: Legacy BIOS                                           | :heavy_check_mark: |
| Boot: UEFI                                                  | :x:                |
| Disk partitioning style: MBR                                | :heavy_check_mark: |
| Disk partitioning style: GPT                                | :x:                |
| Exported disk image: vmdk                                   | :heavy_check_mark: |
| Exported disk image: vdi                                    | :heavy_check_mark: |
| Exported disk image: raw                                    | :heavy_check_mark: |
| kernel <-> app and app <-> app isolation                    | :heavy_check_mark: |
| opearating-mode: real mode (boot-loader + realmode-library) | :heavy_check_mark: |
| opearating-mode: protected mode (kernel + user application) | :heavy_check_mark: |
| driver: disk IO                                             | :heavy_check_mark: |
| driver: PIC                                                 | :heavy_check_mark: |
| driver: PIT                                                 | :heavy_check_mark: |
| driver: Keyboard                                            | :heavy_check_mark: |
| file-system: FFS (in-house) (read-only)                     | :heavy_check_mark: |
| interrupt handler                                           | :heavy_check_mark: |
| syscall: text console IO                                    | :heavy_check_mark: |
| syscall: graphics                                           | :heavy_check_mark: |
| syscall: keyboard                                           | :heavy_check_mark: |
| syscall: file handlers                                      | :heavy_check_mark: |
| process-scheduler: Round Robin                              | :heavy_check_mark: |
| file-system: FFS (custom) (read-only)                       | :heavy_check_mark: |
| display: text-mode                                          | :heavy_check_mark: |
| display: graphics-vga 320x200 256 colors                    | :heavy_check_mark: |
| concurrency: Multiprocessing                                | :heavy_check_mark: |
| concurrency: Multithreading                                 | :x:                |
| parallelism                                                 | :x:                |
| security in scope                                           | :x:                |
| networking                                                  | :x:                |
| app-build: Support C                                        | :heavy_check_mark: |
| app-build: Support C++                                      | :heavy_check_mark: |
| app-build: C/C++ Standard Libraries (as needed)             | :heavy_check_mark: |
| app-runtime: Heap memory allocation                         | :heavy_check_mark: |
| application: calculator                                     | :heavy_check_mark: |
| application: cat                                            | :heavy_check_mark: |
| application: desktop                                        | :heavy_check_mark: |
| application: echo                                           | :heavy_check_mark: |
| application: forkbomb                                       | :heavy_check_mark: |
| application: logo                                           | :heavy_check_mark: |
| application: ls                                             | :heavy_check_mark: |
| application: ping-pong                                      | :heavy_check_mark: |
| application: sh (terminal)                                  | :heavy_check_mark: |
| application: simplecpp (example)                            | :heavy_check_mark: |
| application: more                                           | :heavy_check_mark: |
| application: multiprocessing (example)                      | :heavy_check_mark: |
| application: tictactoe                                      | :heavy_check_mark: |
| application: `Your's? Yes, raise a PR`                      | :heavy_check_mark: |


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


### Want to Contribute?

[CONTRIBUTING.md](CONTRIBUTING.md)


