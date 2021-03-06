#ifndef TESTS_H
#define TESTS_H

#include "common.h"
#include "nstring.h"
#include "nbuf.h"
#include "nmap.h"
#include "blist.h"
#include "types.h"
#include "nqueue.h"
#include "utils.h"

//#define VERBOSE

void test_nstr_create() {
    NString nstr = {};
    int N = 100;
    const char *abc = "abc";
    const char *dbc = "dbc";
    const char *xyz = "xyz";

    for (int i = 0; i < N; i++) {
        nstr_push(&nstr, abc);
        nstr_push(&nstr, dbc);
        nstr_push(&nstr, xyz);
    }

    assert(nstr_len(&nstr) == 900);

    NString nstr2 = nstr_create("Hello, world!");
    nstr_copy(&nstr2, &nstr);
    assert(nstr_len(&nstr2) == 900);

    NString nstr3 = nstr_create("Lorem ipsum dollor! aaaaaaaaaaaaaaaa");
    nstr_copy(&nstr2, &nstr3);
    assert(nstr_len(&nstr2) == nstr_len(&nstr3));

    nstr_clear(&nstr);
    assert(nstr_empty(&nstr));

    nstr_free(&nstr3);
    nstr_free(&nstr2);
    nstr_free(&nstr);
}


void test_nstr_compare() {
    NString a = nstr_create("Test");
    NString b = nstr_create("Test");

    assert(nstr_compare(&a, &b) == 0);

    NString c = {};
    nstr_copy(&c, &b);

    assert(nstr_compare(&c, &b) == 0);

    NString x = nstr_create("Test");
    NString y = nstr_create("Text");

    assert(nstr_compare(&x, &y) < 0);

    nstr_free(&a);
    nstr_free(&b);
    nstr_free(&c);
    nstr_free(&x);
    nstr_free(&y);

}

void test_nstr_substr() {

    NString nstr = nstr_create("Testbar");
    NString ret = nstr_substr(&nstr, 0, 2);

    NString nstr2 = nstr_create("Aaaxyzfooclassic.hero.whatever");
    NString sub = nstr_substr(&nstr2,4,8);
#ifdef VERBOSE
    nstr_print(&sub);
    nstr_print(&ret);
#endif

    assert(nstr_compare_raw(&ret, "Te") == 0);
    assert(nstr_compare_raw(&sub, "yzfo") == 0);

#ifdef VERBOSE
    NString test = nstr_substr(&nstr, 21, 2);
    nstr_print(&test);
#endif 
    nstr_free(&nstr);
    nstr_free(&nstr2);
    nstr_free(&sub);
    nstr_free(&ret);
}

void test_nstr() {

    test_nstr_create();
    test_nstr_compare();
    test_nstr_substr();

    printf("test string: Passed\n");
}

void test_narr() {

    int *arr = NULL;
    for (int i = 0; i < 1024; i++) {
        nbuf_push(arr, i);
    }

    assert(1024 == nbuf_len(arr));
    for (int *it = nbuf_begin(arr); it != nbuf_end(arr); ++it) {
#ifdef VERBOSE
        printf("%d\n", *it);
#endif
    }

    nbuf_free(arr);
    printf("test array: Passed\n"); 
}

void test_nmap() {

    NMap data = nmap_init();
    for (size_t i = 0; i < 1024; i++) {
        nmap_put(&data, (void *)i, (void *)(i + 1));
    }

    for (size_t i = 1; i < 1024; i++) {
        void *val = nmap_get(&data, (void *)i);
        assert(val == (void *)(i+1));
    }

    Iterable it = nmap_iter(&data);
    for (size_t i = 0; i < it.index; i++) {
#ifdef VERBOSE
        Pair p = it.pairs[i];
        printf("Pair <%d, %d>\n", (int)p.key, (int)p.value);
#endif
    }

    free_iter(&it);
    nmap_free(&data);

    printf("test map: Passed\n");
}

void test_blist() {
    BList list = {0};
    for (size_t i = 1; i < 10; i++) {
        blist_push_back(&list, (void *)i);
    }

    assert(9 == blist_len(&list));

    int value = 11;
    blist_insert(&list, blist_next(blist_front(&list)), (void *)value);
    blist_insert(&list, blist_back(&list), (void *)value);

    for (size_t i = 50; i < 55; i++) {
        blist_push_front(&list, (void *)i);
    }

    int search = 52;
    Node *found = blist_find(&list, (void *)search);
    assert(found->item == search);
    if (found != NULL) {
        blist_remove(&list, found);
    }
    
    Node *node = blist_front(&list);
    while (node != NULL) {
#ifdef VERBOSE
        printf("Item: %ld\n", node->item);
#endif
        node = blist_next(node);
    }

   blist_free(&list);
   assert(0 == blist_len(&list));

   printf("test blist: Passed\n");
}

void test_types() {
    Any any;
    int8_t i8 = 10;
    uint8_t u8 = 10;
    int16_t i16 = 10;
    uint16_t u16 = 10;
    int32_t i32 = 10;
    uint32_t u32 = 10;
    int64_t i64 = 10;
    uint64_t u64 = 10;
    float f32 = 10.123;
    double f64 = 10.123;
    
    int8_t i8_result; 
    any_wrap(&any, i8, int8_t);
    any_unwrap(&any, &i8_result);
    assert(i8_result == i8);   

    uint8_t u8_result; 
    any_wrap(&any, u8, uint8_t);
    any_unwrap(&any, &u8_result);
    assert(u8_result == u8);   
    
    int16_t i16_result; 
    any_wrap(&any, i16, int16_t);
    any_unwrap(&any, &i16_result);
    assert(i16_result == i16);   

    uint16_t u16_result; 
    any_wrap(&any, u16, uint16_t);
    any_unwrap(&any, &u16_result);
    assert(u16_result == u16);   

    int32_t i32_result; 
    any_wrap(&any, i32, int32_t);
    any_unwrap(&any, &i32_result);
    assert(i32_result == i32);   

    uint32_t u32_result; 
    any_wrap(&any, u32, uint32_t);
    any_unwrap(&any, &u32_result);
    assert(u32_result == u32);   

    int64_t i64_result; 
    any_wrap(&any, i64, int64_t);
    any_unwrap(&any, &i64_result);
    assert(i64_result == i64);   

    uint64_t u64_result; 
    any_wrap(&any, u64, uint64_t);
    any_unwrap(&any, &u64_result);
    assert(u64_result == u64);   
    
    float f32_result; 
    any_wrap(&any, f32, float);
    any_unwrap(&any, &f32_result);
    assert(f32_result == f32);   

    double f64_result; 
    any_wrap(&any, f64, double);
    any_unwrap(&any, &f64_result);
    assert(f64_result == f64);   

#ifdef VERBOSE

    NString nstr = nstr_create("Foo");
    NMap nmap = {0};
    nmap_put(&nmap, "key", "value");
    BList list = {0};
    blist_push_back(&list, "item");

    NString nstr_result = {0};
    NMap nmap_result = {0};
    BList blist_result = {0};

    any_wrap(&any, &nstr, NString);
    any_unwrap(&any, &nstr_result);
    nstr_print(&nstr_result);

    any_wrap(&any, &nmap, NMap);
    any_unwrap(&any, &nmap_result);

    any_wrap(&any, &list, BList);
    any_unwrap(&any, &blist_result);

#endif

    printf("test types: Passed\n");
}

void test_nqueue() {
    NQueue *queue = nqueue_create();

    assert(nqueue_size(queue) == 0);
    assert(nqueue_head(queue) == NULL);

    nqueue_push(queue, 1);

    assert(nqueue_size(queue) == 1);
    assert(nqueue_head(queue) == 1);

    nqueue_pop(queue);

    assert(nqueue_size(queue) == 0);
    assert(nqueue_head(queue) == NULL);

    nqueue_push(queue, 3);
    nqueue_push(queue, 2);
    nqueue_push(queue, 1);

    assert(nqueue_size(queue) == 3);
    assert(nqueue_head(queue) == 3);

    nqueue_pop(queue);

    assert(nqueue_size(queue) == 2);
    assert(nqueue_head(queue) == 2);

    nqueue_pop(queue);

    assert(nqueue_size(queue) == 1);
    assert(nqueue_head(queue) == 1);

    nqueue_pop(queue);

    assert(nqueue_size(queue) == 0);
    assert(nqueue_head(queue) == 0);

    nqueue_free(queue);

    printf("test queue: Passed\n");
}

void test_utils() {
#if 0
   printf("Print name\n");
   NString name = {0};
   int fd = fileno(stdin);
   get_line(fd, &name);
   nstr_print(&name);
#endif

   int fd = open("test.txt", O_RDONLY);
   NString line = {};
   defer({ 
           nstr_free(&line);
           close(fd);
           printf("cleaned\n"); 
   });

   while (get_line(fd, &line)) {
       nstr_print(&line);
   }

   nstr_free(&line);
}

void run_tests() {
    test_types();
    test_blist();
    test_nstr();
    test_narr();
    test_nmap();
    test_nqueue();
    test_utils();
}

#endif
