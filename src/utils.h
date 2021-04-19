#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/*****************************************************************/
/*********************** Misc utils *******************************/
/*****************************************************************/
#if defined(__clang__) || defined(__GNUC__)

#define defer__merge(a,b) a##b
#define defer__var(a) defer__merge(defer_scopevar_, a)
#define defer__func(a) defer__merge(defer_scopefunc_, a)
#define defer(block) void defer__func(__LINE__)(int *a) block; __attribute__((cleanup(defer__func(__LINE__)))) int defer__var(__LINE__)

#endif

/*****************************************************************/
/*********************** I/O utils *******************************/
/*****************************************************************/

ssize_t get_line(int fd, NString *str) {
    nstr_clear(str);

    char ch[1];
    ssize_t len;
    while ((len = read(fd, ch, sizeof(ch))) > 0) {
        if (ch[0] == '\n' || ch[0] == EOF)
            return len;

         nstr_push(str, ch);
    }

    return len;
}

#endif
