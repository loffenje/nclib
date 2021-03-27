#ifndef BLIST_H
#define BLIST_H

#include "common.h"

struct Node {
    struct Node *prev;
    struct Node *next;
    uint64_t item;
};

typedef struct Node Node;

struct BList {
    struct Node *_head;
    struct Node *_node;
    size_t _len;
};

typedef struct BList BList;

extern inline Node *blist_front(BList *list) {
    return list->_head;
};

extern inline Node *blist_back(BList *list) {
    Node *back = list->_head;
    while (back->next != NULL)
        back = back->next;
    
    return back;
}

void blist_insert_u64(BList *list, Node *node, uint64_t item) {
    Node *new_node = xmalloc(sizeof(Node));
    new_node->item = item;
    if (!node) {
        new_node->next = NULL;
        new_node->prev = NULL;
        
        list->_head = list->_node = new_node;
        goto exit;
    }

    new_node->next = node->next;

    node->next = new_node;
    new_node->prev = node;
    if (new_node->next != NULL)
        new_node->next->prev = new_node;

 exit:
    ++list->_len;
    return;
}

void blist_push_back_u64(BList *list, uint64_t item) {

    if (!list->_head) {
        blist_insert_u64(list, NULL, item); 
        return;
    }

    Node *back = blist_back(list);
    blist_insert_u64(list, back, item);
}


void blist_push_front_u64(BList *list, uint64_t item) {
    
    if (!list->_head) {
        blist_insert_u64(list, NULL, item);
        return;
    }

    Node *node = xmalloc(sizeof(Node));
    node->prev = NULL;
    node->next = list->_head;
    node->item = item;

    ++list->_len;
    
    if (!node->next) {
        node->next->prev = node;
    }
    
    list->_head = list->_node = node;
}

void blist_remove(BList *list, Node *node) {
    
    if (list->_head == NULL || node == NULL) 
        return;

    if (list->_head == node)
        list->_head = node->next;

    if (node->next != NULL) 
        node->next->prev = node->prev;
    

    if (node->prev != NULL) 
        node->prev->next = node->next;

    free(node);
    --list->_len;
}

Node *blist_find_u64(BList *list, uint64_t item) {
    if (list->_len == 0)
        return NULL;
    
    if (list->_len == 1 && list->_head->item == item)
        return list->_head;


    Node *found = NULL;
    Node *node = list->_head;
    while (node->next != NULL) {
        if (node->item == item) {
            found = node;
            break;
        }

        node = node->next;
    }

    return found;
}

extern inline Node *blist_next(Node *node) {
    Node *result = NULL;
    if (node != NULL)
        result = node->next;

    return result;
}

extern inline Node *blist_prev(Node *node) {
    Node *result = NULL;
    if (node != NULL)
        result = node->prev;

    return result;
}

extern inline Node *blist_find(BList *list, const void *item) {
    return blist_find_u64(list, (uint64_t)(uintptr_t)item);
}

extern inline void blist_push_back(BList *list, void *item) {
    blist_push_back_u64(list, (uint64_t)(uintptr_t)item);  
}

extern inline void blist_push_front(BList *list, void *item) {
    blist_push_front_u64(list, (uint64_t)(uintptr_t)item);
}

extern inline void blist_insert(BList *list, Node *node, void *item) {
    blist_insert_u64(list, node, (uint64_t)(uintptr_t)item);
}

extern inline size_t blist_len(BList *list) {
    return list->_len;
}


#endif

