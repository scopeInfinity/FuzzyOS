set $CS_BASE = 0x20000
set $DS_BASE = 0x20000
set $SS_BASE = 0x20000

# config
set architecture i386:x86-64
set pagination off
set prompt \033[32mgdb$ \033[0m

# layout
define cb
    b *($CS_BASE+$arg0)
end

define cprintstr
    x/s $DS_BASE+$arg0
end

define cprintdata
    x/4w $DS_BASE+$arg0
end

define casm
    x/20i ($CS_BASE+$pc)
end

define cstack
    info register esp ebp
    x/24w $SS_BASE+$esp
end

define creg
    # Not needed for now: cr0 cr2 cr3 cr4 cr8 gs_base
    info register eax ebx ecx edx esi edi eflags cs ss ds es fs gs fs_base
end

define _clayout_title
    echo \033[33m---[ $arg0 ]---------------\033[0m\n
end

define clayout
    # clear screen
    echo \033c
    _clayout_title ASM
    casm
    _clayout_title Register
    creg
    _clayout_title Stack
    cstack
    _clayout_title Command
end

define cc
    # shorthand for cresetscreen
    clayout
end

# mode
define view_realmode
    set $CS_BASE = 0x0000
    set $DS_BASE = 0x0000
    set $SS_BASE = 0x0000
    clayout
end

define view_kernelmode
    set $CS_BASE = 0xC000
    set $DS_BASE = 0xC000
    set $SS_BASE = 0xC000
    clayout
end

define view_usermode
    set $CS_BASE = 0x20000
    set $DS_BASE = 0x20000
    set $SS_BASE = 0x20000
    clayout
end

# override existing shorthand
define si
    stepi
    cc
end
define ni
    nexti
    cc
end
define c
    continue
    cc
end
