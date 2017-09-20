#ifndef _BASIC_VALUE_H_
#define _BASIC_VALUE_H_

#include <stdint.h>

typedef union basic_value {
    int64_t l;
    uint64_t u;
    double d;
    const char *s;
    void *p;
} basic_value_t;

#define BASIC2L(_b) ((_b).l)
#define BASIC2U(_b) ((_b).u)
#define BASIC2D(_b) ((_b).d)
#define BASIC2P(_b, _t) ((_t)((_b).p))

#define L2BASIC(_l) ({basic_value_t v; v.l = _l; v;})
#define U2BASIC(_u) ({basic_value_t v; v.u = _u; v;})
#define D2BASIC(_d) ({basic_value_t v; v.d = _d; v;})
#define P2BASIC(_p) ({basic_value_t v; v.p = (void*)_p; v;})

static const basic_value_t _basic_value_null = {0};

#define BASIC_NULL _basic_value_null
#define BASIC_IS_NULL(b) ((b).p == NULL)

#endif //_BASIC_VALUE_H_
