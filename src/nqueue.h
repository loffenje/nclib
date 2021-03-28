#ifndef NQUEUE_H
#define NQUEUE_H
#include "common.h"

typedef struct NQNode {
    struct NQNode *_next;
    struct NQNode *_prev;

    uint64_t value;
} NQNode;

typedef struct NQueue {
    NQNode *_head;
    NQNode *_tail;

    int _size;
} NQueue;

NQueue *nqueue_create() {
    NQueue *queue = xmalloc(sizeof(NQueue));
    NQNode *head = xmalloc(sizeof(NQNode));
    NQNode *tail = xmalloc(sizeof(NQNode));

    head->_next = tail;
    tail->_prev = head;

    queue->_head = head;
    queue->_tail = tail;
    queue->_size = 0;

    return queue;
}

extern inline int nqueue_size(NQueue *queue) {
    return queue->_size;
}

uint64_t nqueue_head(NQueue *queue) {
    if (nqueue_size(queue) <= 0) return NULL;

    return queue->_head->_next->value;
}

void nqueue_push(NQueue *queue, uint64_t value) {
    NQNode *new_node = xmalloc(sizeof(NQNode));
    new_node->_next = queue->_tail;
    new_node->value = value;

    if (queue->_head->_next == NULL) { // queue is empty
        queue->_head->_next = new_node;
        queue->_tail->_prev = new_node;

        new_node->_prev = queue->_head;
    } else {
        queue->_tail->_prev->_next = new_node;
        queue->_tail->_prev = new_node;

        new_node->_prev = queue->_tail->_prev;
        new_node->_next = queue->_tail;
    }

    queue->_size++;
}

uint64_t nqueue_pop(NQueue *queue) {
    if (nqueue_size(queue) <= 0) return NULL;

    NQNode *popped_node = queue->_head->_next;

    if (popped_node->_next == queue->_tail) {
        queue->_head->_next = queue->_tail;
        queue->_tail->_prev = queue->_head;
    } else {
        queue->_head->_next = popped_node->_next;
        popped_node->_next->_prev = queue->_head;
    }

    uint64_t value = popped_node->value;

    free(popped_node);

    queue->_size--;

    return value;
}

void nqueue_free(NQueue *queue) {
    if (queue == NULL) return;
    
    free(queue->_head);
    free(queue->_tail);

    free(queue);
}

#endif
