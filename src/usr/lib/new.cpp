#include <new.h>
#include <stdio.h>
#include <stdlib.h>

void *operator new(std::size_t size) { return std::malloc(size); }

void *operator new[](std::size_t size) { return ::operator new(size); }

void operator delete(void *ptr) { std::free(ptr); }

void operator delete[](void *ptr) { ::operator delete(ptr); }

void *operator new(std::size_t s, void *ptr) { return ptr; }
