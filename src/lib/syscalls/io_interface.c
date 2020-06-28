#ifndef __LIB_SYSCALLS_IO_INTERFACE_C
#define __LIB_SYSCALLS_IO_INTERFACE_C

#ifdef IMPORT_IO_INTERFACE_PROTECTED
#include <lib/syscalls/io_interface_protected.c>

#ifdef __IMPORT_IO_INTERFACE_DEFINED
#error "Exactly one among IMPORT_IO_INTERFACE_{BIOS,IMPORT_IO_INTERFACE_PROTECTED} must be defined to use IO library."
#endif
#define __IMPORT_IO_INTERFACE_DEFINED
#endif

#ifdef IMPORT_IO_INTERFACE_BIOS
#include <lib/syscalls/io_interface_bios.c>

#ifdef __IMPORT_IO_INTERFACE_DEFINED
#error "Exactly one among IMPORT_IO_INTERFACE_{BIOS,IMPORT_IO_INTERFACE_PROTECTED} must be defined to use IO library."
#endif
#define __IMPORT_IO_INTERFACE_DEFINED
#endif

#ifndef __IMPORT_IO_INTERFACE_DEFINED
#error "Exactly one among IMPORT_IO_INTERFACE_{BIOS,IMPORT_IO_INTERFACE_PROTECTED} must be defined to use IO library."
#endif

#endif