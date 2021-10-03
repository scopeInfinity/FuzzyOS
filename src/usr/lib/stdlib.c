#include <stdlib.h>
#include <stddef.h>
#include <process.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <math.h>

int min(int a, int b) {
    return (a<b)?a:b;
}

int max(int a, int b) {
    return (a>b)?a:b;
}

int abs(int a) {
    return (a>=0)?a:-a;
}

int atoi(const char *s) {
    int neg = 0;
    if ((*s) == '-' || (*s) == '+') {
        neg = ((*s) == '-');
        s++;
    }
    int num = 0;
    while((*s)!='\0') {
        num=num*10+(*s)-'0';
        s++;
    }
    if(neg) num = -num;
    return num;
}

void itoa(int num, char *s, int base) {
    // TODO: add assert on non-positive base.
    if(num==0) {
        *(s++)='0';
        *(s++)='\0';
        return;
    }
    if (base == 10 && num<0) {
        *(s++) = '-';
        num=-num;
    }

    int digit_count = 0;
    int bnum = num;
    while(bnum) {
        digit_count++;
        bnum/=base;
    }

    *(s+digit_count) = '\0';
    while((digit_count--)>0) {
        int c = num%base;
        num /= base;
        *(s+digit_count) = (c<10)?'0'+c:'A'-10+c;
    }
}

void ftoa(double num, char *s) {
    // not a good implementation
    if (isnan(num)) {
        *(s++) = 'n';
        *(s++) = 'a';
        *(s++) = 'n';
        *(s++) = '\0';
        return;
    }
    if (num < 0) {
        *(s++) = '-';
        num = - num;
    }
    if(num==0) {
        *(s++)='0';
        *(s++)='\0';
        return;
    }
    int exp = 0;
    while(num>=1) {
        num /= 10;
        exp++;
    }
    while(num<0.1) {
        num *= 10;
        exp--;
    }

    *(s++)='0';
    *(s++)='.';

    for(int i=0;i<8;i++) {
        num *= 10;
        int msb = num;
        *(s++)='0'+msb;
        num -= msb;
        if (num < 1e-16) {
            break;
        }
    }

    if(exp!=0) {
        *(s++) = 'e';
        if(exp>0) *(s++) = '+';
        itoa(exp, s, 10);
    } else {
        *(s++) = '\0';
    }
}

static void (*_at_exit_handler)(void) = NULL;
void atexit(void (*handler)(void)) {
    // supports only one handler
    _at_exit_handler = handler;
}

static void at_exit_trigger() {
    // make sure to kill the program even if exit_handler crashes
    static int at_exit_triggered = 0;
    if (at_exit_triggered == 0) {
        // first call of this program
        at_exit_triggered = 1;
        if(_at_exit_handler) {
            _at_exit_handler();
        }
    } else {
        // call sequence
        // exit -> at_exit_trigger -> _at_exit_handler -> exit (possibly due to crash)
        // do nothing
    }
}

void exit(int status) {
    at_exit_trigger();
    SYSCALL_A2(SYSCALL_PROCESS, SYSCALL_PROCESS_SUB_EXIT, status);
    // process should be marked as ready to kill, wait util it's killed.
    // process yield should be enough complete kill.
    yield();
    while(1);
}

/*
malloc implementation
 - linear allocation based on first come first served
 - allocate first free block >= requested size

pros
 - easy to implement as the initial implementation
cons
 - malloc is slow
   - as we are not using virtual addressing the physical
     allocated memory is limited slowly ness shouldn't be
     noticeable.

memory_layout
-------------
_heap_start:  # static marker defined at link time
heap_entry{is_free, size}
heap_entry{is_free, size}
heap_entry{is_free, size}
...
... heap_entry_count times
... unused memory
_heap_end:  # dynamic marker based on current esp

*/
extern void* get_current_esp();  // defined in stdlib.asm
extern char _heap_start[];  // defined in linker.ld
static int heap_entry_count = 0;
static const int heap_stack_safety_gap = 1024; // keep 1 kb free between stack and heap

// benchmarking
static int benchmark_heap_inuse = 0;
static int benchmark_heap_area = 0;

int benchmark_get_heap_usage() {
    // heap memory in use
    return benchmark_heap_inuse;
}

int benchmark_get_heap_area() {
    // overall memory area ever occupied by heap area
    return benchmark_heap_area;
}

static union heap_entry {
    // header of each allocated or unallocated block
    struct {
        uint32_t size;  // including the header size
        uint8_t is_free;
    } content;
    uint32_t __padding[2];  // force heap_entry size to be 8 bytes.
};

static union heap_entry *malloc_allocate_new_block(union heap_entry *block, size_t size) {
    void* max_loc = get_current_esp()-heap_stack_safety_gap-size;
    if ((void*)block>max_loc) {
        // panic if no memory to allocate
        printf("failed to allocate more memory\n");
        exit(-1);
    }
    benchmark_heap_area = max(benchmark_heap_area, (int)(((void*)block) + size - (void*)_heap_start));

    block->content.size = size;
    block->content.is_free = 1;  // new block is free at init.
    heap_entry_count++;
    return block;
}

static union heap_entry *malloc_split_block(union heap_entry *block, size_t size) {
    // returns the first one of the two splitted blocks

    // [ --------- old-free-block-------------- ]
    // [ allocating-block]  [new-left-over-block]
    size_t left_over_size = block->content.size - size;
    if (left_over_size < sizeof(union heap_entry)+8) {
        // do not split block if left-over block for less than 8 bytes allocation.
        // i.e. do not split block
        return block;
    }
    // allocate new-left-over-block
    union heap_entry *block_second = malloc_allocate_new_block((union heap_entry *)(((void*)block)+size), left_over_size);
    // resize first block
    block->content.size = size;
    return block;
}

static void malloc_merge_onfree(union heap_entry *block) {
    // merge recently free block to neighboring block
    // if they are free are also free to form large free
    // block.
    if (!block->content.is_free) return;

    // TODO(scopeinfinity): implementation pending.
}

static union heap_entry *malloc_find_freeblock(size_t size) {
    // size includes header size
    void* loc = (void*)_heap_start; // start
    int block_id = 0;
    while(1) {

        union heap_entry *block = loc;
        if (block_id==heap_entry_count) {
            // create new node
            return malloc_allocate_new_block(block, size);
        }
        if ((!block->content.is_free) || block->content.size < size) {
            // block is not free
            // not sufficient memory in this block
            loc += block->content.size;
            block_id++;
            continue;
        }
        // returns the block after split or no-split
        return malloc_split_block(block, size);
    }
}

void* malloc(size_t size) {
    // allocates in chunk of 4
    size = ((size + 3) >> 2 ) << 2;
    // allocate header
    size += sizeof(union heap_entry);

    union heap_entry *header = malloc_find_freeblock(size);

    benchmark_heap_inuse += size;
    header->content.is_free = 0;  // false
    return (((void*)header)+sizeof(union heap_entry));
}

void free(void* ptr) {
    if(ptr == NULL) return;
    // current version of malloc is non-optimal and doesn't
    // do any free operation.
    union heap_entry *header = ptr-sizeof(union heap_entry);
    if (header->content.is_free!=0) {
        // trying to free unallocated memory
        return;
    }
    header->content.is_free = 1;  // true
    size_t size = header->content.size;
    benchmark_heap_inuse -= size;
    malloc_merge_onfree(header);
}
