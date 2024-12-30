#pragma once
/*
 * Every macro has the prefix of da_
 * These macros expect a struct with the following fields
 *
 * // Note: <Type> can be anything (int, float, or some struct, etc...)
 * struct Dynamic_Array {
 *      <Type> *items;
 *      size_t size;
 *      size_t count;
 * }
 */

#ifdef DA_IMPLEMENTATION
#define DA_INITIAL_SIZE 8

#define da_init(da)         \
    do {                    \
        (da)->items = NULL; \
        (da)->size = 0;     \
        (da)->count = 0;    \
    } while(0)

#define da_free(da)         \
    do {                    \
        free((da)->items);  \
        (da)->items = NULL; \
        (da)->size = 0;     \
        (da)->count = 0;    \
    } while(0)

#define da_append(da, item)                                                         \
    do {                                                                            \
        if ((da)->count >= (da)->size)                                              \
        {                                                                           \
            (da)->size = (da)->size==0 ? DA_INITIAL_SIZE : (da)->size*2;            \
            (da)->items = realloc((da)->items, (da)->size * sizeof(*(da)->items));  \
            assert((da)->items != NULL);                                            \
        }                                                                           \
        (da)->items[(da)->count++] = (item);                                        \
    } while(0)

#define da_remove(da)                      \
    do {                                   \
        if ((da)->count > 0) (da)->count--;\
    } while(0)

#define da_reserve(da, needed_size)                                                \
    do {                                                                           \
        if((da)->size < needed_size)                                               \
        {                                                                          \
            (da)->size = needed_size;                                              \
            (da)->items = realloc((da)->items, (da)->size * sizeof(*(da)->items)); \
            assert((da)->items != NULL);                                           \
        }                                                                          \
    } while(0)
#endif
