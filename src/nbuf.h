#ifndef NBUF_H
#define NBUF_H


#include <limits.h>

#ifndef SIZE_MAX
#  define SIZE_MAX __SIZE_MAX__
#endif

struct NBuffer {
    size_t _len;
    size_t _cap;
    char _buf[];
};

typedef struct NBuffer NBuffer;

#define nbuf__hdr(b) ((NBuffer *)((char *)(b) - offsetof(NBuffer, _buf)))

#define nbuf_len(b) ((b) ? nbuf__hdr(b)->_len : 0)
#define nbuf_cap(b) ((b) ? nbuf__hdr(b)->_cap : 0)
#define nbuf_begin(b) ((b))
#define nbuf_end(b) ((b) + nbuf_len(b))
#define nbuf_sizeof(b) ((b) ? nbuf_len(b)*sizeof(*b) : 0)

#define nbuf_free(b) ((b) ? (free(nbuf__hdr(b)), (b) = NULL) : 0)
#define nbuf_fit(b, n) ((n) <= nbuf_cap(b) ? 0 : ((b) = nbuf__grow((b), (n), sizeof(*(b)))))
#define nbuf_push(b, ...) (nbuf_fit((b), 1 + nbuf_len(b)), (b)[nbuf__hdr(b)->_len++] = (__VA_ARGS__))
#define nbuf_clear(b) ((b) ? nbuf__hdr(b)->_len = 0 : 0)

void *nbuf__grow(const void *buf, size_t new_len, size_t elem_size) {
    assert(nbuf_cap(buf) <= (SIZE_MAX - 1)/2);
    size_t new_cap = CLAMP_MIN(2*nbuf_cap(buf), MAX(new_len, 16));
    assert(new_len <= new_cap);
    assert(new_cap <= (SIZE_MAX - offsetof(NBuffer, _buf))/elem_size);
    size_t new_size = offsetof(NBuffer, _buf) + new_cap*elem_size;
    NBuffer *new_hdr;
    if (buf) {
        new_hdr = xrealloc(nbuf__hdr(buf), new_size);
    } else {
        new_hdr = xmalloc(new_size);
        new_hdr->_len = 0;
    }
   
    new_hdr->_cap = new_cap;
    
    return new_hdr->_buf;
}


#endif
