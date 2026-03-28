#include <stdio.h>
#define HEADERS
#include "test_all.cpp"
#undef HEADERS

#define TEST(name) test = name;
#define ASSERT(expr)                            \
    do {                                        \
        assertion = #expr;                      \
        file = __FILE__;                        \
        line = __LINE__;                        \
        if(expr) putchar('.'); else goto fail;  \
    } while(0)

int main() {
    const char* test        = "";
    const char* assertion   = "";
    const char* file        = "";
    int line                = 0;

    #define TESTS
    #include "test_all.cpp"
    #undef TESTS

    putchar('\n');
    return 0;

fail:
    printf("!\nTest failed at %s:%d\n\t%s: %s\n",
        file, line,
        test, assertion);
    return 1;
}