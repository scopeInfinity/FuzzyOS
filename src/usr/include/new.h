#pragma once

#include <stddef.h>

void *operator new(std::size_t size);
void *operator new[](std::size_t size);
void operator delete(void *ptr);
void operator delete[](void *ptr);

void *operator new(std::size_t size, void *);
