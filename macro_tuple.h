/**
 * @author: luyuhuang
 * @brief: tuple using macro implementation
 */

#ifndef _MACRO_TUPLE_H_
#define _MACRO_TUPLE_H_

#include <stdlib.h>

#define TUPLE_1(t1) \
    struct{t1 _1;}
#define TUPLE_2(t1, t2) \
    struct{t1 _1; t2 _2;}
#define TUPLE_3(t1, t2, t3) \
    struct{t1 _1; t2 _2; t3 _3;}
#define TUPLE_4(t1, t2, t3, t4) \
    struct{t1 _1; t2 _2; t3 _3; t4 _4;}
#define TUPLE_5(t1, t2, t3, t4, t5) \
    struct{t1 _1; t2 _2; t3 _3; t4 _4; t5 _5;}
#define TUPLE_6(t1, t2, t3, t4, t5, t6) \
    struct{t1 _1; t2 _2; t3 _3; t4 _4; t5 _5; t6 _6;}

typedef void *tuple_t;

#define NEW_TUPLE_1(v1)                                                 \
    ({                                                                  \
        typedef TUPLE_1(                                                \
                __typeof__(v1)                                          \
                ) _nEW_TUPLE_1_type;                                    \
        _nEW_TUPLE_1_type *_nEW_TUPLE_1_t =                             \
            (_nEW_TUPLE_1_type*)malloc(sizeof(_nEW_TUPLE_1_type));      \
        _nEW_TUPLE_1_t->_1 = v1;                                        \
        (void*)_nEW_TUPLE_1_t;                                          \
     })

#define NEW_TUPLE_2(v1, v2)                                             \
    ({                                                                  \
        typedef TUPLE_2(                                                \
                __typeof__(v1),                                         \
                __typeof__(v2)                                          \
                ) _nEW_TUPLE_2_type;                                    \
        _nEW_TUPLE_2_type *_nEW_TUPLE_2_t =                             \
            (_nEW_TUPLE_2_type*)malloc(sizeof(_nEW_TUPLE_2_type));      \
        _nEW_TUPLE_2_t->_1 = v1;                                        \
        _nEW_TUPLE_2_t->_2 = v2;                                        \
        (void*)_nEW_TUPLE_2_t;                                          \
     })


#define NEW_TUPLE_3(v1, v2, v3)                                         \
    ({                                                                  \
        typedef TUPLE_3(                                                \
                __typeof__(v1),                                         \
                __typeof__(v2),                                         \
                __typeof__(v3)                                          \
                ) _nEW_TUPLE_3_type;                                    \
        _nEW_TUPLE_3_type *_nEW_TUPLE_3_t =                             \
            (_nEW_TUPLE_3_type*)malloc(sizeof(_nEW_TUPLE_3_type));      \
        _nEW_TUPLE_3_t->_1 = v1;                                        \
        _nEW_TUPLE_3_t->_2 = v2;                                        \
        _nEW_TUPLE_3_t->_3 = v3;                                        \
        (void*)_nEW_TUPLE_3_t;                                          \
     })

#define NEW_TUPLE_4(v1, v2, v3, v4)                                     \
    ({                                                                  \
        typedef TUPLE_4(                                                \
                __typeof__(v1),                                         \
                __typeof__(v2),                                         \
                __typeof__(v3),                                         \
                __typeof__(v4)                                          \
                ) _nEW_TUPLE_4_type;                                    \
        _nEW_TUPLE_4_type *_nEW_TUPLE_4_t =                             \
            (_nEW_TUPLE_4_type*)malloc(sizeof(_nEW_TUPLE_4_type));      \
        _nEW_TUPLE_4_t->_1 = v1;                                        \
        _nEW_TUPLE_4_t->_2 = v2;                                        \
        _nEW_TUPLE_4_t->_3 = v3;                                        \
        _nEW_TUPLE_4_t->_4 = v4;                                        \
        (void*)_nEW_TUPLE_4_t;                                          \
     })

#define NEW_TUPLE_5(v1, v2, v3, v4, v5)                                 \
    ({                                                                  \
        typedef TUPLE_5(                                                \
                __typeof__(v1),                                         \
                __typeof__(v2),                                         \
                __typeof__(v3),                                         \
                __typeof__(v4),                                         \
                __typeof__(v5)                                          \
                ) _nEW_TUPLE_5_type;                                    \
        _nEW_TUPLE_5_type *_nEW_TUPLE_5_t =                             \
            (_nEW_TUPLE_5_type*)malloc(sizeof(_nEW_TUPLE_5_type));      \
        _nEW_TUPLE_5_t->_1 = v1;                                        \
        _nEW_TUPLE_5_t->_2 = v2;                                        \
        _nEW_TUPLE_5_t->_3 = v3;                                        \
        _nEW_TUPLE_5_t->_4 = v4;                                        \
        _nEW_TUPLE_5_t->_5 = v5;                                        \
        (void*)_nEW_TUPLE_5_t;                                          \
     })

#define NEW_TUPLE_6(v1, v2, v3, v4, v5, v6)                             \
    ({                                                                  \
        typedef TUPLE_6(                                                \
                __typeof__(v1),                                         \
                __typeof__(v2),                                         \
                __typeof__(v3),                                         \
                __typeof__(v4),                                         \
                __typeof__(v5),                                         \
                __typeof__(v6)                                          \
                ) _NEW_TUPLE_6_type;                                    \
        _NEW_TUPLE_6_type *_NEW_TUPLE_6_t =                             \
            (_NEW_TUPLE_6_type*)malloc(sizeof(_NEW_TUPLE_6_type));      \
        _NEW_TUPLE_6_t->_1 = v1;                                        \
        _NEW_TUPLE_6_t->_2 = v2;                                        \
        _NEW_TUPLE_6_t->_3 = v3;                                        \
        _NEW_TUPLE_6_t->_4 = v4;                                        \
        _NEW_TUPLE_6_t->_5 = v5;                                        \
        _NEW_TUPLE_6_t->_6 = v6;                                        \
        (void*)_NEW_TUPLE_6_t;                                          \
     })


#define GET_TUPLE_1(t, v1)                                              \
    do {                                                                \
         typedef TUPLE_1(                                               \
                __typeof__(v1)                                          \
                ) _gET_TUPLE_1_type;                                    \
         _gET_TUPLE_1_type *_gET_TUPLE_1_tuple = (_gET_TUPLE_1_type*)t; \
         v1 = _gET_TUPLE_1_tuple->_1;                                   \
    } while (0)

#define GET_TUPLE_2(t, v1, v2)                                          \
    do {                                                                \
         typedef TUPLE_2(                                               \
                __typeof__(v1),                                         \
                __typeof__(v2)                                          \
                ) _gET_TUPLE_2_type;                                    \
         _gET_TUPLE_2_type *_gET_TUPLE_2_tuple = (_gET_TUPLE_2_type*)t; \
         v1 = _gET_TUPLE_2_tuple->_1;                                   \
         v2 = _gET_TUPLE_2_tuple->_2;                                   \
    } while (0)



#define GET_TUPLE_3(t, v1, v2, v3)                                      \
    do {                                                                \
         typedef TUPLE_3(                                               \
                __typeof__(v1),                                         \
                __typeof__(v2),                                         \
                __typeof__(v3)                                          \
                ) _gET_TUPLE_3_type;                                    \
         _gET_TUPLE_3_type *_gET_TUPLE_3_tuple = (_gET_TUPLE_3_type*)t; \
         v1 = _gET_TUPLE_3_tuple->_1;                                   \
         v2 = _gET_TUPLE_3_tuple->_2;                                   \
         v3 = _gET_TUPLE_3_tuple->_3;                                   \
    } while (0)

#define GET_TUPLE_4(t, v1, v2, v3, v4)                                  \
    do {                                                                \
         typedef TUPLE_4(                                               \
                __typeof__(v1),                                         \
                __typeof__(v2),                                         \
                __typeof__(v3),                                         \
                __typeof__(v4)                                          \
                ) _gET_TUPLE_4_type;                                    \
         _gET_TUPLE_4_type *_gET_TUPLE_4_tuple = (_gET_TUPLE_4_type*)t; \
         v1 = _gET_TUPLE_4_tuple->_1;                                   \
         v2 = _gET_TUPLE_4_tuple->_2;                                   \
         v3 = _gET_TUPLE_4_tuple->_3;                                   \
         v4 = _gET_TUPLE_4_tuple->_4;                                   \
    } while (0)

#define GET_TUPLE_5(t, v1, v2, v3, v4, v5)                              \
    do {                                                                \
         typedef TUPLE_5(                                               \
                __typeof__(v1),                                         \
                __typeof__(v2),                                         \
                __typeof__(v3),                                         \
                __typeof__(v4),                                         \
                __typeof__(v5)                                          \
                ) _gET_TUPLE_5_type;                                    \
         _gET_TUPLE_5_type *_gET_TUPLE_5_tuple = (_gET_TUPLE_5_type*)t; \
         v1 = _gET_TUPLE_5_tuple->_1;                                   \
         v2 = _gET_TUPLE_5_tuple->_2;                                   \
         v3 = _gET_TUPLE_5_tuple->_3;                                   \
         v4 = _gET_TUPLE_5_tuple->_4;                                   \
         v5 = _gET_TUPLE_5_tuple->_5;                                   \
    } while (0)

#define GET_TUPLE_6(t, v1, v2, v3, v4, v5, v6)                          \
    do {                                                                \
         typedef TUPLE_6(                                               \
                __typeof__(v1),                                         \
                __typeof__(v2),                                         \
                __typeof__(v3),                                         \
                __typeof__(v4),                                         \
                __typeof__(v5),                                         \
                __typeof__(v6)                                          \
                ) _GET_TUPLE_6_type;                                    \
         _GET_TUPLE_6_type *_GET_TUPLE_6_tuple = (_GET_TUPLE_6_type*)t; \
         v1 = _GET_TUPLE_6_tuple->_1;                                   \
         v2 = _GET_TUPLE_6_tuple->_2;                                   \
         v3 = _GET_TUPLE_6_tuple->_3;                                   \
         v4 = _GET_TUPLE_6_tuple->_4;                                   \
         v5 = _GET_TUPLE_6_tuple->_5;                                   \
         v6 = _GET_TUPLE_6_tuple->_6;                                   \
    } while (0)



#define DELETE_TUPLE(t) free(t)

#endif //_MACRO_TUPLE_H_
