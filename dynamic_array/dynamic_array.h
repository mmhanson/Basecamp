/*
 * A generic dynamic array implemented from scratch in C89.
 *
 * The array doubles in size whenever there is no room for another element. It
 * halfs in size when it is less than 30% full.
 * The genericness of this dynamic array is implemented with macros. To declare
 * and define a dynamic array and all its operations, use the 'TODO' macro.
 * Contracts when EQUAL TO OR BELOW 0.3 load (load=size/capacity).
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
#define DEFINE_DYNAMIC_ARRAY(T)          \
DEFINE_DYNAMIC_ARRAY_STRUCT(T)          \
DEFINE_DYNAMIC_ARRAY_CTOR(T)             \
DEFINE_DYNAMIC_ARRAY_DTOR(T)             \
DEFINE_DYNAMIC_ARRAY_ADD(T)             \
DEFINE_DYNAMIC_ARRAY_ADD_AT(T)   \
DEFINE_DYNAMIC_ARRAY_INSERT(T)   \
DEFINE_DYNAMIC_ARRAY_EXPAND(T)           \
DEFINE_DYNAMIC_ARRAY_REMOVE_FIRST(T)    \
DEFINE_DYNAMIC_ARRAY_REMOVE(T)          \
DEFINE_DYNAMIC_ARRAY_CONTRACT(T)        \
DEFINE_DYNAMIC_ARRAY_REMOVE_AT(T)       \
DEFINE_DYNAMIC_ARRAY_CONTAINS(T)        \

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
 * Free all memory associated with a dynamic array.
 */
#define DEFINE_DYNAMIC_ARRAY_DTOR(T)                           \
void dynamic_array_destruct(T##_DYNAMIC_ARRAY *dyn_arr)    \
{                                                            \
    free(dyn_arr->array);                                      \
    free(dyn_arr);                                               \
}

/*
 * Add an element to the end of a dynamic array.
 *
 * Returns 0 if failure, 1 if success.
 */
#define DEFINE_DYNAMIC_ARRAY_ADD(T)                           \
static int dynamic_array_insert(T##_DYNAMIC_ARRAY *dyn_arr, T elem, int i); \
int dynamic_array_add(T##_DYNAMIC_ARRAY *dyn_arr, T elem)     \
{                                                             \
    return dynamic_array_insert(dyn_arr, elem, dyn_arr->size);             \
}

/*
 * Add an element at the i-th index of a dynamic array.
 *
 * elem will be the new i-th element of the array.
 */
#define DEFINE_DYNAMIC_ARRAY_ADD_AT(T)                           \
static int dynamic_array_insert(T##_DYNAMIC_ARRAY *dyn_arr, T elem, int i); \
int dynamic_array_add_at(T##_DYNAMIC_ARRAY *dyn_arr, T elem, int i) \
{                                                                    \
    return dynamic_array_insert(dyn_arr, elem, i);                         \
}

/*
 * Insert an element at the i-th index of a dynami array.
 * Does not increment size. Automatically expands if necessary.
 * Helper methods for add, add_at operations.
 * Return 0 if error, positive value if success.
 */
#define DEFINE_DYNAMIC_ARRAY_INSERT(T)                                  \
static int dynamic_array_expand(T##_DYNAMIC_ARRAY *dyn_arr);         \
static int dynamic_array_insert(T##_DYNAMIC_ARRAY *dyn_arr, T elem, int i)  \
{                                                                     \
    int idx;                                                               \
    int status;                                                       \
    T *array;                                                               \
                                                                               \
    if (dyn_arr->load_factor == 1)                                         \
    {                                                                    \
        status = dynamic_array_expand(dyn_arr);                              \
        if (status == 0)                                                    \
        {                                                                       \
            /* Pass allocation error up. */                                  \
            return status;                                                  \
        }                                                                     \
    }                                                                       \
                                                                             \
    /* Move all elements in [i+1..dyn_arr->size) forward one index */       \
    array = dyn_arr->array;                                                \
    for (idx = dyn_arr->size; idx > i; idx--)                                \
    {                                                                         \
        array[idx] = array[idx - 1];                                           \
    }                                                                         \
                                                                              \
    array[i] = elem;                                                          \
    dyn_arr->size += 1;                                                      \
    dyn_arr->load_factor = ((float)dyn_arr->size) / ((float)dyn_arr->capacity); \
                                                                              \
    return 1;                                                               \
}

/*
 * Remove the occurrence of an element in the array.
 *
 * Return 1 if the remove was successful, return 0 if not.
 * Array is unmodified if element not contained. Returns 0.
 */
#define DEFINE_DYNAMIC_ARRAY_REMOVE_FIRST(T)                         \
static int dynamic_array_remove(T##_DYNAMIC_ARRAY *dyn_arr, int rem_idx);       \
int dynamic_array_remove_first(T##_DYNAMIC_ARRAY *dyn_arr, T elem)    \
{                                                               \
    int idx;                                                      \
                                                                 \
    for (idx = 0; idx < dyn_arr->size; idx++)                       \
    {                                                           \
        if ((dyn_arr->array)[idx] == elem)                       \
        {                                                         \
            return dynamic_array_remove(dyn_arr, idx);                            \
        }                                                           \
    }                                                              \
    return 0;                                                       \
}

/*
 * Remove the i-th element of a dynamic array.
 */
#define DEFINE_DYNAMIC_ARRAY_REMOVE_AT(T)                                \
static int dynamic_array_remove(T##_DYNAMIC_ARRAY *dyn_arr, int idx);   \
int dynamic_array_remove_at(T##_DYNAMIC_ARRAY *dyn_arr, int i)          \
{                                                                         \
    return dynamic_array_remove(dyn_arr, i);                               \
}                                                                       \

/*
 * Test if the array contains a key.
 *
 * Return 1 if it does, 0 if it doesnt.
 */
#define DEFINE_DYNAMIC_ARRAY_CONTAINS(T)                              \
int dynamic_array_contains(T##_DYNAMIC_ARRAY *dyn_arr, T elem)        \
{                                                                     \
    int idx;                                                           \
    T *array;                                                          \
                                                                        \
    array = dyn_arr->array;                                             \
    for (idx = 0; idx < dyn_arr->size; idx++)                           \
    {                                                                     \
        if (array[idx] == elem)                                         \
        {                                                              \
            return 1;                                                      \
        }                                                                 \
    }                                                                   \
    return 0;                                                             \
}

/*
 * Allocate a new array (double the capacity) and copy the elements over.
 *
 * Returns 0 if there is an error allocating the new array. Returns 1 otherwise.
 */
#define DEFINE_DYNAMIC_ARRAY_EXPAND(T)                                      \
static int dynamic_array_expand(T##_DYNAMIC_ARRAY *dyn_arr)                   \
{                                                                               \
    T *old_array;                                                               \
    int new_capacity;                       \
    int idx;                                                                    \
                                                                                \
    old_array = dyn_arr->array;                                                 \
    new_capacity = 2 * dyn_arr->capacity;                \
    dyn_arr->array = calloc(new_capacity, sizeof(T));                  \
    if (dyn_arr->array == NULL)                                                 \
    {                                                                           \
        return 0;                                                               \
    }                                                                           \
    for (idx = 0; idx <= dyn_arr->size; idx++)                                  \
    {                                                                           \
        dyn_arr->array[idx] = old_array[idx];                                   \
    }                                                                           \
    free(old_array); \
                                                                                \
    dyn_arr->capacity = new_capacity;                                  \
    dyn_arr->load_factor = ((float)dyn_arr->size) / ((float)dyn_arr->capacity); \
                                                                                \
    return 1;                                                                   \
}

/*
 * Allocate a new array (half the capacity) and copy the elements over.
 *
 * Returns 0 if there is an error allocating the new array. Returns 1 otherwise.
 * Note that the array must have, at most, half load or undefined behavior.
 */
#define DEFINE_DYNAMIC_ARRAY_CONTRACT(T)                                    \
static int dynamic_array_contract(T##_DYNAMIC_ARRAY *dyn_arr)       \
{                                                                          \
    T *old_array;                                                               \
    int new_capacity;                                                     \
    int idx;                                                                    \
                                                                                \
    old_array = dyn_arr->array;                                                 \
    new_capacity = 0.5 * dyn_arr->capacity;                                     \
    dyn_arr->array = calloc(new_capacity, sizeof(T));                           \
    if (dyn_arr->array == NULL)                                                 \
    {                                                                           \
        return 0; /* error allocating new array */                           \
    }                                                                           \
    for (idx = 0; idx <= dyn_arr->size; idx++)                                  \
    {                                                                           \
        dyn_arr->array[idx] = old_array[idx];                                   \
    }                                                                           \
    free(old_array);                                                   \
                                                                                \
    dyn_arr->capacity = new_capacity;                                  \
    dyn_arr->load_factor = ((float)dyn_arr->size) / ((float)dyn_arr->capacity); \
                                                                                \
    return 1;                                                                   \
}

/*
 * Remove the element at the ith index of an array.
 *
 * Automatically contracts the array if it dips below 30% load.
 */
// TODO zero the last element?
// TODO update size?
#define DEFINE_DYNAMIC_ARRAY_REMOVE(T)                       \
static int dynamic_array_contract(T##_DYNAMIC_ARRAY *dyn_arr);      \
static int dynamic_array_remove(T##_DYNAMIC_ARRAY *dyn_arr, int rem_idx) \
{                                                           \
    int idx;                                                 \
    T *array;                                                  \
                                                               \
    /* Copy every element in [i+1..size) back one index */    \
    array = dyn_arr->array;                                    \
    for (idx = rem_idx + 1; idx < dyn_arr->size; idx++)        \
    {                                                         \
        array[idx - 1] = array[idx];                            \
    }                                                          \
                                                                \
    dyn_arr->size -= 1;                                         \
    dyn_arr->load_factor = ((float)dyn_arr->size) / ((float)dyn_arr->capacity); \
                                                                  \
    /* Only half if array has doubled before */                     \
    if (dyn_arr->load_factor <= 0.3 && dyn_arr->capacity > 10)      \
    {                                                              \
        return dynamic_array_contract(dyn_arr); /* 1 if reallocation successful, 0 if not */ \
    }                                                           \
                                                                  \
    return 1;                                                      \
}

#endif
















