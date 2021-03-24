#ifndef NSTRING_H
#define NSTRING_H


#define VERSION 0.1

#define ALIGN_DOWN(n, a) ((n) & ~((a) - 1))
#define ALIGN_UP(n, a) ALIGN_DOWN((n) + (a) - 1, (a))
#define NSTR_STR(nstr) (nstr->_flushed ? nstr->_memory : nstr->_buf)
#define NSTR_AT(nstr, i) (NSTR_STR(nstr) + i)

#define BUF_SIZE 16

typedef int32_t b32;

struct NString {
    size_t _len;
    size_t _cap;
    b32 _flushed;
    b32 _init;
    b32 _freed;
    char *_memory;
    char _buf[BUF_SIZE];
    int32_t _buf_space;
};

typedef struct NString NString;


static void nstr__drain_buf(NString *nstr, const char *source, size_t source_len) {
    nstr->_buf_space -= source_len;
    if (nstr->_init) {
        memcpy(nstr->_buf + nstr->_len, source, source_len + 1);
        nstr->_len += source_len;
    } else {
        memcpy(nstr->_buf, source, source_len + 1);
        nstr->_cap = BUF_SIZE;
        nstr->_len = source_len;
    }

    nstr->_buf[nstr->_len] = '\0';
}

static void nstr__flush_buf(NString *nstr, size_t source_len) {

    size_t size = source_len + nstr->_cap;
    int align_n = 8;
    size = ALIGN_UP(size, align_n);
    
    int buf_len = BUF_SIZE - nstr->_buf_space;
    nstr->_memory = xmalloc(size);
    
    memcpy(nstr->_memory, nstr->_buf, buf_len + 1);
    nstr->_memory[buf_len] = '\0';

    memset(nstr->_buf, 0, sizeof(nstr->_buf));

    nstr->_buf_space = 0;
    nstr->_flushed = 1;
    nstr->_cap = size;
}

static int nstr__sso_copy(NString *nstr, const char *source) {
    size_t source_len = strlen(source);
    if (nstr->_buf_space > source_len) {
        nstr__drain_buf(nstr, source, source_len);     
        return 1;
    }

    if (!nstr->_flushed)
        nstr__flush_buf(nstr, source_len);
    
    return 0;
}

NString nstr_create(const char *source) {

    size_t source_len = strlen(source);

    NString nstr = {0};
    nstr._buf_space = BUF_SIZE;
    if (source_len < BUF_SIZE) {
        nstr__drain_buf(&nstr, source, source_len);
        nstr._init = 1;
               
        return nstr;
    } 

    nstr._memory = xmalloc(source_len);
    memcpy(nstr._memory, source, source_len + 1);
    nstr._memory[source_len] = '\0';
    
    nstr._flushed = 1;
    nstr._len = source_len;
    nstr._cap = source_len;
    nstr._init = 1;
    
    return nstr;
}

void nstr_push(NString *nstr, const char *source) {
    if (!nstr->_init) {
        *nstr = nstr_create(source);
        return;
    }
    
    if (nstr__sso_copy(nstr, source))
        return;

    size_t source_len = strlen(source);
    size_t new_len = 0;
    new_len = nstr->_len + source_len;

    if (new_len > nstr->_cap) {
        nstr->_cap *= 2;
        nstr->_memory = xrealloc(nstr->_memory, nstr->_cap);
    }

    memcpy(nstr->_memory + nstr->_len, source, source_len + 1);
    nstr->_memory[new_len] = '\0';
    
    nstr->_len = new_len;
}


void nstr_copy(NString *source, NString *dest) {
    source->_len = dest->_len;
    source->_cap = dest->_cap;
    
    if (source->_flushed) {
        source->_memory = xrealloc(source->_memory, source->_cap);
    } else if (dest->_flushed) {
        source->_memory = xmalloc(source->_cap);
        source->_flushed = 1;
    } else {
        memset(source->_buf, 0, sizeof(source->_buf));
        memcpy(source->_buf, dest->_buf, sizeof(source->_buf));
        source->_buf[source->_len] = '\0';
        return;
    }

    memcpy(source->_memory, NSTR_STR(dest), source->_len + 1); 
    source->_memory[source->_len] = '\0';
}

void nstr_free(NString *nstr) {
    nstr->_len = 0;
    nstr->_cap = 0;
    nstr->_init = 0;
    if (nstr->_flushed && !nstr->_freed) {
      free(nstr->_memory);
      nstr->_freed = 1;
    }

    nstr->_memory = NULL;
}

NString nstr_substr(NString *nstr, size_t pos, size_t count) {
    size_t size = count - pos;

    NString result = {};
    if (size > nstr->_len) {
        return result;
    }

    char substr[size];
    memcpy(substr, NSTR_AT(nstr, pos), sizeof(substr));
    substr[size] = '\0';

    if (nstr->_flushed) {
        result._memory = xmalloc(size);
        result._flushed = 1;

        memcpy(result._memory, substr, sizeof(substr));
        result._memory[size] = '\0';
    } else {
        memcpy(result._buf, substr, sizeof(substr));
        result._buf[size] = '\0';
    }
        
    result._len = size;
    result._cap = size;
    result._init = 1;

    return result;
}


extern inline bool nstr_starts_with(NString *nstr, const char *str) {
    return memcmp(NSTR_STR(nstr), str, strlen(str)) == 0;
}

extern inline int nstr_compare(NString *nstr1, NString *nstr2) {
   if (nstr1->_len != nstr2->_len)
       return -1;

   return memcmp(NSTR_STR(nstr1), NSTR_STR(nstr2), nstr1->_len); 
}

extern inline int nstr_compare_raw(NString *nstr, const char *str) {
   if (nstr->_len != strlen(str))
        return -1;

   return memcmp(NSTR_STR(nstr), str, nstr->_len); 
}

extern inline void nstr_clear(NString *nstr) {
    memset(NSTR_STR(nstr), 0, nstr->_cap);
    nstr->_len = 0;
}


extern inline void nstr_print(NString *nstr) {
    fprintf(stdout, "%s\n", NSTR_STR(nstr));
}

extern inline size_t nstr_len(NString *nstr) {
    return nstr->_len;
}

extern inline size_t nstr_cap(NString *nstr) {
    return nstr->_cap;
}

extern inline bool nstr_empty(NString *nstr) {
    return nstr->_len == 0;
}

#endif
