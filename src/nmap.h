#ifndef NMAP_H
#define NMAP_H

#ifndef MAP_CAP
#define MAP_CAP 100
#endif

struct Bucket {
    void *key;
    void *value;
    int hash;
    struct Bucket *next;
};

typedef struct Bucket Bucket;

struct NMap {
    Bucket **_buckets;
    size_t _bucket_count;
    int (*_hash)(void *key);
    bool (*_cmp)(void *a, void *b);
    size_t _size;
};

typedef struct NMap NMap;


int nhash(void *key) {
    int hash = 0;
    char *s = (char *)key;
    size_t len = strlen(s);
    for (size_t i = 0; i < len; i++) {
        hash = hash * 31 + *s;
        s++;
    }

    return hash; 
}

NMap nmap_create() {
    NMap map = {};
    size_t load = MAP_CAP * 4 / 3;
    map._bucket_count = 1;
    while (map._bucket_count <= load) {
        map._bucket_count <<= 1;
    }

    map._buckets = xcalloc(map._bucket_count, sizeof(Bucket*));
    map._size = 0;
    map._hash = nhash;
  
    return map;
}


void nmap_set_hash_func(NMap *nmap, int (*hash)(void *key)) {
    nmap->_hash = hash;
}

void nmap_set_cmp_func(NMap *nmap, bool (*cmp)(void *a, void *b)) {
    nmap->_cmp = cmp;
}

bool nmap_cmp_int(void *key_a, void *key_b) {
    int a = *((int*) key_a);
    int b = *((int*) key_b);

    return a == b;
}

bool nmap_cmp_char(void *key_a, void *key_b) {

    char *a = (char *) key_a;
    char *b = (char *) key_b;

    return strcmp(a, b) == 0;
}

static inline int nmap__hash(NMap *nmap, void *key) {
    int value = nmap->_hash(key);
    
    value += ~(value << 9);
    value ^= (((unsigned int) value) >> 14);
    value += (value << 4);
    value ^= (((unsigned int) value) >> 10);

    return value;
}

inline size_t nmap_size(NMap *nmap) {
    return nmap->_size;
}

static inline size_t nmap__get_index(int bucket_count, int hash) {
    size_t index = ((size_t)hash) & (bucket_count - 1);
    
    return index;
}

static inline bool nmap__cmp(NMap *nmap, Bucket *bucket, void *key, int hash) {
    if (bucket->key == key) {
        return true;
    }

    if (bucket->hash != hash) {
        return false;
    }

    return nmap->_cmp(bucket->key, key);
}

static Bucket *nmap__create_bucket(void *key, void *value, int hash) {
    Bucket *bucket = xmalloc(sizeof(Bucket));
    bucket->key = key;
    bucket->value = value;
    bucket->hash = hash;

    return bucket;
} 

static void nmap__grow(NMap *nmap) {
    if (nmap->_size > (nmap->_bucket_count * 3 / 4)) {
        size_t new_bucket_count = nmap->_bucket_count << 1;
        Bucket **new_buckets = xcalloc(new_bucket_count, sizeof(Bucket *));

        for (size_t i = 0; i < nmap->_bucket_count; i++) {
            Bucket *bucket = nmap->_buckets[i];
            while (bucket != NULL) {
                Bucket *next = bucket->next;
                size_t index = nmap__get_index(new_bucket_count, bucket->hash);
                bucket->next = new_buckets[index];
                new_buckets[index] = bucket;
                bucket = next;
            }
        }

        free(nmap->_buckets);
        nmap->_buckets = new_buckets;
        nmap->_bucket_count = new_bucket_count;
    }
}

void nmap_put(NMap *nmap, void *key, void *value) {
    int hash = nmap__hash(nmap, key);
    size_t index = nmap__get_index(nmap->_bucket_count, hash);

    Bucket **bucket = &(nmap->_buckets[index]);
    for (;;) {
        Bucket *current = *bucket;
        if (current == NULL) {
            *bucket = nmap__create_bucket(key, value, hash);
            if (*bucket == NULL) {
                return;
            }
            
            nmap->_size++;
            nmap__grow(nmap);
            
            return;
        }

        if (nmap__cmp(nmap, current, key, hash)) {
            current->value = value;
            return;
        }

        bucket = &current->next;
    }
}

void nmap_del(NMap *nmap, void *key) {
    int hash = nmap__hash(nmap, key);
    size_t index = nmap__get_index(nmap->_bucket_count, hash);

    Bucket **bucket = &(nmap->_buckets[index]);
    Bucket *current;
    while ((current = *bucket) != NULL) {
        if (nmap__cmp(nmap, current, key, hash)) {
            *bucket = current->next;
            free(current);
            current = NULL;
            nmap->_size--;
            return;
        }

        bucket = &current->next;
    }
}

void nmap_free(NMap *nmap) {
    for (size_t i = 0; i < nmap->_bucket_count; i++) {
        Bucket *bucket = nmap->_buckets[i];
        while (bucket != NULL) {
            Bucket *next = bucket->next;
            free(bucket);
            bucket = next;
        }
    }

    free(nmap->_buckets);
}

void *nmap_get(NMap *nmap, void *key) {
    int hash = nmap__hash(nmap, key);
    size_t index = nmap__get_index(nmap->_bucket_count, hash);
    Bucket *bucket = nmap->_buckets[index];
    while (bucket != NULL) {
        if (nmap__cmp(nmap, bucket, key, hash)) {
            return bucket->value;
        }

        bucket = bucket->value;
    }

    return NULL;
}

#endif
