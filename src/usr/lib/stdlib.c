#include <limits.h>
#include <math.h>
#include <process.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>

// heap memory parameter

// Full search on heap allocated memory can be a bit slow.
// #define HEAP_MALLOC_SEARCH_MOST_APPROPRIATE_BLOCK

// During reallocating a block should extra space be created
// as a new heap block.
#define HEAP_MALLOC_SPLIT_ON_REALLOCATING_BLOCK

// During free operating should a block merge with free neighbours.
#define HEAP_MALLOC_FREE_MERGE_NEIGHBOURS

int min(int a, int b) { return (a < b) ? a : b; }

int max(int a, int b) { return (a > b) ? a : b; }

int abs(int a) { return (a >= 0) ? a : -a; }

int atoi(const char *s) {
    int neg = 0;
    if ((*s) == '-' || (*s) == '+') {
        neg = ((*s) == '-');
        s++;
    }
    int num = 0;
    while ((*s) != '\0') {
        num = num * 10 + (*s) - '0';
        s++;
    }
    if (neg)
        num = -num;
    return num;
}

void itoa(int num, char *s, int base) {
    // TODO: add assert on non-positive base.
    if (num == 0) {
        *(s++) = '0';
        *(s++) = '\0';
        return;
    }
    if (base == 10 && num < 0) {
        *(s++) = '-';
        num = -num;
    }

    int digit_count = 0;
    int bnum = num;
    while (bnum) {
        digit_count++;
        bnum /= base;
    }

    *(s + digit_count) = '\0';
    while ((digit_count--) > 0) {
        int c = num % base;
        num /= base;
        *(s + digit_count) = (c < 10) ? '0' + c : 'A' - 10 + c;
    }
}

void ftoa(double num, char *s, int afterpoint) {
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
        num = -num;
    }
    if (num == 0) {
        *(s++) = '0';
        *(s++) = '\0';
        return;
    }
    int exp = 0;
    while (num >= 1) {
        num /= 10;
        exp++;
    }
    while (num < 0.1) {
        num *= 10;
        exp--;
    }

    *(s++) = '0';
    *(s++) = '.';

    for (int i = 0; i < afterpoint; i++) {
        num *= 10;
        int msb = num;
        *(s++) = '0' + msb;
        num -= msb;
        if (num < 1e-16) {
            break;
        }
    }

    if (exp != 0) {
        *(s++) = 'e';
        if (exp > 0)
            *(s++) = '+';
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
        if (_at_exit_handler) {
            _at_exit_handler();
        }
    } else {
        // call sequence
        // exit -> at_exit_trigger -> _at_exit_handler -> exit (possibly due to
        // crash) do nothing
    }
}

void exit(int status) {
    at_exit_trigger();
    SYSCALL_A2(SYSCALL_PROCESS, SYSCALL_PROCESS_SUB_EXIT, status);
    // process should be marked as ready to kill, wait util it's killed.
    // process yield should be enough complete kill.
    yield();
    while (1)
        ;
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
heap_entry{HEAP_BLOCK_FIRST,     size=0}
heap_entry{HEAP_BLOCK_ALLOCATED, size}
heap_entry{HEAP_BLOCK_FREE,      size}
heap_entry{HEAP_BLOCK_ALLOCATED, size}
...
heap_entry{HEAP_BLOCK_LAST,      size=0}
... unused memory
_heap_end:  # dynamic marker based on current esp

*/

static union heap_entry {
    // header of each allocated or unallocated block
    struct {
        // including the header size
        uint32_t size;      // size of current block
        uint32_t prev_size; // size of prev block for doubly linked list
        uint8_t state;
    } content;
    uint32_t __padding[3]; // force heap_entry size to be 12 bytes.
};

#define HEAP_HEADER_SIZE (sizeof(union heap_entry))

#define HEAP_BLOCK_FREE 0
#define HEAP_BLOCK_ALLOCATED 1
#define HEAP_BLOCK_FIRST 2
#define HEAP_BLOCK_LAST 3

extern void *get_current_esp(); // defined in stdlib.asm
// symbol name is defined in linker.ld
extern char _heap_start[];
static const int heap_stack_safety_gap =
    1024;                        // keep 1 kb free between stack and heap
static int heap_initialized = 0; // false

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

static inline void heap_panic(const char *msg) {
    printf("heap memory err: %s\n", msg);
    exit(-1);
}

static inline void heap_assert(int is_not_panic, const char *msg) {
    if (!is_not_panic) {
        heap_panic(msg);
    }
    return;
}

static inline void heap_may_init() {
    if (!heap_initialized) {
        union heap_entry *first = &(((union heap_entry *)_heap_start)[0]);
        union heap_entry *last = &(((union heap_entry *)_heap_start)[1]);
        first->content.state = HEAP_BLOCK_FIRST;
        last->content.state = HEAP_BLOCK_LAST;
        first->content.prev_size = 0;
        last->content.size = HEAP_HEADER_SIZE;
        first->content.size = last->content.prev_size = HEAP_HEADER_SIZE;
        heap_initialized = 1;
    }
}

static inline union heap_entry *heap_next_block(union heap_entry *current) {
    return (((void *)current) + current->content.size);
}

static inline union heap_entry *heap_prev_block(union heap_entry *current) {
    return (((void *)current) - current->content.prev_size);
}

static inline union heap_entry *heap_push_back(union heap_entry *olast,
                                               size_t new_size) {
    heap_assert(olast->content.state == HEAP_BLOCK_LAST,
                "heap_push_back not called on last");
    void *_heap_end = get_current_esp() - heap_stack_safety_gap;

    olast->content.state = HEAP_BLOCK_FREE;
    olast->content.size = new_size;
    union heap_entry *nlast = heap_next_block(olast);

    benchmark_heap_area =
        max(benchmark_heap_area, (int)nlast - (int)_heap_start);

    // panic if no memory to allocate
    // using nlast instead of heap_next_block(nlast) as diff
    // should be negligible compared to heap_stack_safety_gap.
    heap_assert(_heap_end > (void *)nlast, "failed to allocate more memory");

    // new last block
    nlast->content.state = HEAP_BLOCK_LAST;
    nlast->content.size = HEAP_HEADER_SIZE;
    nlast->content.prev_size = new_size;
    return olast;
}

static inline union heap_entry *heap_freeblock_split(union heap_entry *node,
                                                     size_t first_size) {
    heap_assert(node->content.state == HEAP_BLOCK_FREE,
                "heap_freeblock_split not called on free block");

    // before: [ -------- old-free-block--------- ] [out-block]
    // after : [ new-first-block][new-second-block] [out-block]

    size_t old_size = node->content.size;
    size_t second_block_size = old_size - first_size;

    heap_assert(first_size <= old_size,
                "heap_freeblock_split !(first_size<=old_size)");

    if (second_block_size < HEAP_HEADER_SIZE + 4) {
        // do not split block if second_block_data_size  < 4
        return node;
    }

    union heap_entry *out_block = heap_next_block(node);
    node->content.size = first_size;
    union heap_entry *second = heap_next_block(node);
    second->content.state = second->content.state;
    second->content.size = second_block_size;
    second->content.prev_size = node->content.size;

    out_block->content.prev_size = second->content.size;
    return node;
}

static inline union heap_entry *heap_freeblocks_merge(union heap_entry *node) {
    heap_assert(node->content.state == HEAP_BLOCK_FREE,
                "heap_freeblocks_merge not called on free block");

    // before: [prev] [ node ] [next]
    // after : [prev] [ node ] [next] OR
    // after : [prev +  node ] [next] OR
    // after : [prev] [ node +  next] OR
    // after : [prev +  node +  next]

    union heap_entry *prev = heap_prev_block(node);
    union heap_entry *next = heap_next_block(node);
    if (prev->content.state == HEAP_BLOCK_FREE) {
        // merge
        prev->content.size += node->content.size;
        next->content.prev_size = prev->content.size;
        node = prev;
    }
    if (next->content.state == HEAP_BLOCK_FREE) {
        // merge
        union heap_entry *next_2 = heap_next_block(next);
        node->content.size += next->content.size;
        next_2->content.prev_size = node->content.size;
    }
    return node;
}

static inline union heap_entry *heap_get_free_block(size_t new_size) {
    union heap_entry *block = (union heap_entry *)_heap_start; // start
    uint32_t most_appropriate_value = UINT_MAX;
    union heap_entry *most_appropriate_block = NULL;
    while (1) {
        if (block->content.state == HEAP_BLOCK_LAST) {
            break;
        }
        if (block->content.state == HEAP_BLOCK_FREE &&
            block->content.size >= new_size) {
            // block is free and have sufficient memory in this block
            uint32_t my_value = (block->content.size - new_size);
            if (my_value < most_appropriate_value) {
                most_appropriate_value = my_value;
                most_appropriate_block = block;
#ifndef HEAP_MALLOC_SEARCH_MOST_APPROPRIATE_BLOCK
                break;
#endif
            }
        }
        block = heap_next_block(block);
    }
    if (most_appropriate_block == NULL) {
        return heap_push_back(block, new_size);
    } else {
#ifdef HEAP_MALLOC_SPLIT_ON_REALLOCATING_BLOCK
        return heap_freeblock_split(most_appropriate_block, new_size);
#else
        return most_appropriate_block;
#endif
    }
}

void *malloc(size_t size) {
    heap_may_init();
    // allocates in chunk of 4
    size = ((size + 3) >> 2) << 2;
    // allocate header
    size += HEAP_HEADER_SIZE;

    union heap_entry *header = heap_get_free_block(size);

    benchmark_heap_inuse += size;
    header->content.state = HEAP_BLOCK_ALLOCATED;
    return (((void *)header) + HEAP_HEADER_SIZE);
}

void free(void *ptr) {
    if (ptr == NULL)
        return;
    heap_may_init();

    // current version of malloc is non-optimal and doesn't
    // do any free operation.
    union heap_entry *header = ptr - HEAP_HEADER_SIZE;
    if (header->content.state != HEAP_BLOCK_ALLOCATED) {
        // trying to free unallocated memory
        return;
    }
    header->content.state = HEAP_BLOCK_FREE;
    benchmark_heap_inuse -= header->content.size;

#ifdef HEAP_MALLOC_FREE_MERGE_NEIGHBOURS
    heap_freeblocks_merge(header);
#endif
}
