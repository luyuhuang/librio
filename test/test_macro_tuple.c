#include "../macro_tuple.h"
#include <stdio.h>


static int _test_macro_tuple()
{
    int _test(tuple_t tuple) {
        int n;
        float f;
        const char *s;

        GET_TUPLE_3(tuple, n, f, s);

        printf("%d, %f, %s\n", n, f, s);
        return 0;
    }

    tuple_t tuple = NEW_TUPLE_3((int)1, (float)3.2f, (const char*)"hello");

    _test(tuple);
    DELETE_TUPLE(tuple);
    return 0;
}

int main()
{
    _test_macro_tuple();
    return 0;
}
