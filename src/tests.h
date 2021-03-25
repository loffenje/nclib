#ifndef TESTS_H
#define TESTS_H

#include "common.h"
#include "nstring.h"
#include "nbuf.h"
#include "nmap.h"

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


    printf("Len: %ld\n", nstr_len(&nstr));
    printf("Cap: %ld\n", nstr_cap(&nstr));

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
    nstr_print(&sub);
    nstr_print(&ret);

    assert(nstr_compare_raw(&ret, "Te") == 0);
    assert(nstr_compare_raw(&sub, "yzfo") == 0);

    NString test = nstr_substr(&nstr, 21, 2);

    nstr_print(&test);
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
    nbuf_push(arr, 20);
    nbuf_push(arr, 30);
    nbuf_push(arr, 40);

    printf("Len: %lu\n", nbuf_len(arr));
    printf("Cap: %lu\n", nbuf_cap(arr));
    for (int *it = nbuf_begin(arr); it != nbuf_end(arr); ++it) {
        printf("%d\n", *it);
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
        Pair p = it.pairs[i];
        (void)p;
        // printf("Pair <%d, %d>\n", (int)p.key, (int)p.value);
    }

    free_iter(&it);
    nmap_free(&data);

    printf("test map: Passed\n");
}

void run_tests() {
    test_nstr();
    test_narr();
    test_nmap();
}

#endif
