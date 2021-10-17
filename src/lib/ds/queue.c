#include <lib/ds/queue.h>

int queue_init(int container[], int size) {
    container[2] = size;
    if (size < 4)
        return 0;
    int *back = &container[0];
    int *front = &container[1];
    *back = *front = -1;
    return 1;
}

void queue_clear(int container[]) {
    int *back = &container[0];
    int *front = &container[1];
    *back = *front = -1;
}

int queue_push(int container[], int data) {
    int *back = &container[0];
    int *front = &container[1];
    int size = container[2];
    if ((*back) == -1) {
        *back = 3;
        *front = 3;
    } else {
        (*back)++;
        if ((*back) >= size) {
            (*back) = 3;
        }
        if ((*back) == (*front)) {
            return 0;
        }
    }
    container[*back] = data;
    return 1;
}

int queue_push_front(int container[], int data) {
    int *back = &container[0];
    int *front = &container[1];
    int size = container[2];
    if ((*back) == -1) {
        *back = 3;
        *front = 3;
    } else {
        (*front)--;
        if ((*front) < 3) {
            (*front) = size - 1;
        }
        if ((*back) == (*front)) {
            return 0;
        }
    }
    container[*front] = data;
    return 1;
}

int queue_front(int container[]) {
    int *front = &container[0];
    return container[*front];
}

int queue_pop(int container[]) {
    int *back = &container[0];
    int *front = &container[1];
    int size = container[2];
    int top = container[*front];
    if ((*front) == (*back)) {
        *front = *back = -1;
        return 0;
    } else {
        (*front)++;
        if ((*front) >= size) {
            (*front) = 3;
        }
        return 1;
    }
}

int queue_size(int container[]) {
    int *back = &container[0];
    int *front = &container[1];
    if ((*back) < 0)
        return 0;
    if ((*back) >= (*front)) {
        return (*back) - (*front) + 1;
    }
    int size = container[2];
    return size - (*front) + (*back) - 1;
}

int queue_capacity(int container[]) {
    int size = container[2];
    return size;
}