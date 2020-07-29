#pragma once
// Note: Container expects size to be atleast 3 more than then
// the effective_size.

// Returns if the queue init is successful.
int queue_init(int container[], int effective_size);
// Clears the queue.
void queue_clear(int container[]);
// Returns if the push is successful.
int queue_push(int container[], int data);
// Returns if the push is successful.
int queue_push_front(int container[], int data);
// Returns the front of the queue.
int queue_front(int container[]);
// Returns if the pop is successful.
int queue_pop(int container[]);
// Returns the number of elements in the queue.
int queue_size(int container[]);
// Returns the queue capacity.
int queue_capacity(int container[]);