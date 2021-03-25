#ifndef NMAP_H
#define NMAP_H

#include "common.h"

struct NMap {
    uint64_t *_keys;
    uint64_t *_values;
    size_t _len;
    size_t _cap;
};

typedef struct NMap NMap;

static uint64_t hash_mix(uint64_t key) {
    key ^= (key >> 33);
    key *= 0xff51afd7ed558ccd;
    key ^= (key >> 33);
    key *= 0xc4ceb9fe1a85ec53;
    key ^= (key >> 33);

    return key;
}

void nmap_grow(NMap *map, size_t cap);

void nmap_put_u64(NMap *map, uint64_t key, uint64_t value) {
    if (!key || !value)
        return;
    
    if (map->_len >= map->_cap)
        nmap_grow(map, map->_cap*2);

    assert(IS_POW2(map->_cap));
    size_t i = (size_t)hash_mix(key);
    for (;;) {
        i &= map->_cap - 1;
        if (!map->_keys[i]) {
            map->_len++;
            map->_keys[i] = key;
            map->_values[i] = value;
            return;
        } else if (map->_keys[i] == key) {
            map->_values[i] = value;
            return;
        }

        i++;
    }
}

NMap nmap_init() {
    size_t cap = 16;
    NMap result = {
        ._keys = xcalloc(cap, sizeof(uint64_t)),
        ._values = xmalloc(cap * sizeof(uint64_t)),
        ._cap = cap
    };

    return result;
}

void nmap_grow(NMap *map, size_t cap) {
    cap = CLAMP_MIN(cap, 16);
    NMap new_map = {
        ._keys = xcalloc(cap, sizeof(uint64_t)),
        ._values = xmalloc(cap * sizeof(uint64_t)),
        ._cap = cap
    };

    for (size_t i = 0; i < map->_cap; i++) {
        if (map->_keys[i])
            nmap_put_u64(&new_map, map->_keys[i], map->_values[i]);
    }

    free(map->_keys);
    free(map->_values);
    *map = new_map;
}

uint64_t nmap_get_u64(NMap *map, uint64_t key) {
    if (map->_len == 0)
        return 0;

    assert(IS_POW2(map->_cap));
    assert(map->_len < map->_cap);
    
    size_t i = (size_t)hash_mix(key);
    for (;;) {
        i &= map->_cap - 1;
        if (map->_keys[i] == key) 
            return map->_values[i];
        else if (!map->_keys[i])
            return 0;

        i++;
    }

    return 0;
}

void nmap_del_u64(NMap *map, uint64_t key) {
    size_t i = (size_t)hash_mix(key);
    for (;;) {
        i &= map->_cap - 1;
        if (map->_keys[i] == key) {
            map->_keys[i] = 0;
            map->_values[i] = 0;
   
            return;
        }
    }
}

void nmap_del(NMap *map, const void *key) {
    nmap_del_u64(map, (uint64_t)(uintptr_t)key);
}

void nmap_put(NMap *map, const void *key, void *value) {
    nmap_put_u64(map, (uint64_t)(uintptr_t)key, (uint64_t)(uintptr_t)value);
}

void *nmap_get(NMap *map, const void *key) {
    void *result = (void *)(uintptr_t)nmap_get_u64(map, (uint64_t)(uintptr_t)key);

    return result;
}

void nmap_free(NMap *map) {
    if (map == NULL)
        return;

    free(map->_keys);
    free(map->_values);
    map->_keys = map->_values = NULL;
    map->_len = map->_cap = 0;
    map = NULL;
}

#endif
