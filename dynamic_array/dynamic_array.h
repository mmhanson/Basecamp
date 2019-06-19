/*
 * A generic dynamic array implemented from scratch in C89.
 *
 * The array doubles in size whenever there is no room for another element. It
 * halfs in size when it is less than 30% full.
 * The genericness of this dynamic array is implemented with macros. To declare
 * and define a dynamic array and all its operations, use the 'TODO' macro.
 *
 * The dynamic array is a struct called 'T_DYNAMIC_ARRAY'.
 *
 * Written by Max Hanson, June 2019.
 */

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdlib.h>

/*
 * Macro to define a dynamic array type called 'T_DYNAMIC_ARRAY' and its ops.
 */
#define DEFINE_DYNAMIC_ARRAY(T) \
DEFINE_DYNAMIC_ARRAY_STRUCT(T)  \
DEFINE_DYNAMIC_ARRAY_CTOR(T)    \
DEFINE_DYNAMIC_ARRAY_ADD(T)     \
DEFINE_DYNAMIC_ARRAY_REALLOCATE(T) 

/*
 * A dynamic array.
 */
#define DEFINE_DYNAMIC_ARRAY_STRUCT(T)      \
typedef struct T##_DYNAMIC_ARRAY_TAG \
{                                    \
    float load_factor;               \
    int size;                        \
    int capacity;                    \
    T *array;                        \
} T##_DYNAMIC_ARRAY;                 \

/*
 * Create an empty dynamic array on the heap.
 *
 * Array size is initially 10. All entries are zeroed out.
 */
#define DEFINE_DYNAMIC_ARRAY_CTOR(T)                                \
T##_DYNAMIC_ARRAY *dynamic_array_construct()                        \
{                                                                   \
    T##_DYNAMIC_ARRAY *dyn_arr = malloc(sizeof(T##_DYNAMIC_ARRAY)); \
    dyn_arr->array = calloc(10, sizeof(T));                         \
    dyn_arr->load_factor = 0;                                       \
    dyn_arr->size = 0;                                              \
    dyn_arr->capacity = 10;                                         \
}

/*
 * Add an element to the dynamic array.
 *
 * Returns 0 if failure, 1 if success.
 */
#define DEFINE_DYNAMIC_ARRAY_ADD(T)                           \
int dynamic_array_add(T##_DYNAMIC_ARRAY *dyn_arr, T elem)     \
{                                                             \
    if (dyn_arr->load_factor == 1)                            \
    {                                                         \
        reallocate(dyn_arr);                                  \
    }                                                         \
    (dyn_arr->array)[dyn_arr->size] = elem;                   \
    dyn_arr->size += 1;                                       \
    dyn_arr->load_factor = ((float)dyn_arr->size) / ((float)dyn_arr->capacity); \
    return 1;                                                 \
}

/*
 * Allocate a new array (double the capacity) and copy the elements over.
 *
 * Returns 0 if there is an error allocating the new array. Returns 1 otherwise.
 */
#define DEFINE_DYNAMIC_ARRAY_REALLOCATE(T)                                      \
static int reallocate(T##_DYNAMIC_ARRAY *dyn_arr)                               \
{                                                                               \
    T *old_array;                                                               \
    int idx;                                                                    \
                                                                                \
    old_array = dyn_arr->array;                                                 \
    dyn_arr->array = calloc(2 * dyn_arr->capacity, sizeof(T));                  \
    if (dyn_arr->array == NULL)                                                 \
    {                                                                           \
        return 0;                                                               \
    }                                                                           \
    for (idx = 0; idx <= dyn_arr->size; idx++)                                  \
    {                                                                           \
        dyn_arr->array[idx] = old_array[idx];                                   \
    }                                                                           \
                                                                                \
    dyn_arr->capacity = 2 * dyn_arr->capacity;                                  \
    dyn_arr->load_factor = ((float)dyn_arr->size) / ((float)dyn_arr->capacity); \
                                                                                \
    return 1;                                                                   \
}

#endif
















