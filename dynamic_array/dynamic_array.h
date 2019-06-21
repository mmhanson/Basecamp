/*
 * A generic dynamic array implemented from scratch in C89.
 *
 * See consts for when array expands and contracts and by how much.
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

// TODO all caps?
static const int init_capacity = 10; /* Initial capacity. */
static const float expansion_point = 1.0; /* If load >= this, array expands */
/* If load <= this, array contracts */
static const float contraction_point = 0.3;
/* Expanded capacity = this * old capacity */
static const float expansion_factor = 2.0;
/* Contracted capacity = this * old capacity */
static const float contraction_factor = 0.5;

/*
 * Macro to define a dynamic array and its operations.
 *
 * The dynamic array type is called 'DynamicArray_T' where 'T' is the parameter
 * given to the marcro. For example, calling 'DEFINE_DYNAMIC_ARRAY(float)' would
 * create a type 'DynamicArray_float'.
 */
#define DEFINE_DYNAMIC_ARRAY(T); \
    DEFINE_DYNAMIC_ARRAY_STRUCT(T); \
    DECLARE_DYNAMIC_ARRAY_HELPERS(T);            \
    DEFINE_DYNAMIC_ARRAY_CTOR(T); \
    DEFINE_DYNAMIC_ARRAY_DTOR(T); \
    DEFINE_DYNAMIC_ARRAY_ADD(T); \
    DEFINE_DYNAMIC_ARRAY_ADD_AT(T); \
    DEFINE_DYNAMIC_ARRAY_REMOVE(T); \
    DEFINE_DYNAMIC_ARRAY_REMOVE_AT(T); \
    DEFINE_DYNAMIC_ARRAY_CONTAINS(T); \
    DEFINE_DYNAMIC_ARRAY_CLEAR(T); \
    DEFINE_DYNAMIC_ARRAY_EXPAND(T); \
    DEFINE_DYNAMIC_ARRAY_CONTRACT(T); \
    DEFINE_DYNAMIC_ARRAY_INSERT_ELEM(T); \
    DEFINE_DYNAMIC_ARRAY_DELETE_ELEM(T); \
    DEFINE_DYNAMIC_ARRAY_RECALC_LOAD(T); \

/*
 * A dynamic array.
 */
#define DEFINE_DYNAMIC_ARRAY_STRUCT(T); \
    typedef struct T##_DYNAMIC_ARRAY_TAG \
    { \
        float load; \
        int size; \
        int capacity; \
        T *array; \
    } DynamicArray_##T;

/*
 * Create an empty dynamic array on the heap.
 *
 * Array size is initially 10. All entries are zeroed out.
 */
#define DEFINE_DYNAMIC_ARRAY_CTOR(T); \
    DynamicArray_##T *dynamic_array_##T##_construct() \
    { \
        DynamicArray_##T *dyn_arr = malloc(sizeof(DynamicArray_##T)); \
        dyn_arr->array = calloc(init_capacity, sizeof(T)); \
        dyn_arr->load = 0; \
        dyn_arr->size = 0; \
        dyn_arr->capacity = init_capacity; \
    }

/*
 * Free all memory associated with a dynamic array.
 */
#define DEFINE_DYNAMIC_ARRAY_DTOR(T);                       \
    void dynamic_array_##T##_destruct(DynamicArray_##T *dyn_arr) \
    { \
        free(dyn_arr->array); \
        free(dyn_arr); \
    }

/*
 * Add an element to the end of a dynamic array.
 *
 * Returns 0 if failure, 1 if success.
 */
#define DEFINE_DYNAMIC_ARRAY_ADD(T); \
    int dynamic_array_##T##_add(DynamicArray_##T *dyn_arr, T elem) \
    { \
        return dynamic_array_##T##_insert_elem(dyn_arr, elem, dyn_arr->size); \
    }

/*
 * Add an element at the i-th index of a dynamic array.
 *
 * elem will be the new i-th element of the array.
 */
#define DEFINE_DYNAMIC_ARRAY_ADD_AT(T); \
    int dynamic_array_##T##_add_at(DynamicArray_##T *dyn_arr, T elem, int i) \
    { \
        return dynamic_array_##T##_insert_elem(dyn_arr, elem, i); \
    }

/*
 * Remove the first occurrence of an element in the array.
 *
 * Return 1 if the remove was successful, return 0 if not.
 * Array is unmodified if element not contained. Returns 0.
 */
#define DEFINE_DYNAMIC_ARRAY_REMOVE(T); \
    int dynamic_array_##T##_remove(DynamicArray_##T *dyn_arr, T elem) \
    { \
        int idx; \
        for (idx = 0; idx < dyn_arr->size; idx++) \
        { \
            if ((dyn_arr->array)[idx] == elem) \
            { \
                return dynamic_array_##T##_delete_elem(dyn_arr, idx); \
            } \
        } \
        return 0; \
    }

/*
 * Remove the i-th element of a dynamic array.
 */
#define DEFINE_DYNAMIC_ARRAY_REMOVE_AT(T); \
    int dynamic_array_##T##_remove_at(DynamicArray_##T *dyn_arr, int i) \
    { \
        return dynamic_array_##T##_delete_elem(dyn_arr, i); \
    }

/*
 * Test if the array contains a key.
 *
 * Return 1 if it does, 0 if it doesnt.
 */
#define DEFINE_DYNAMIC_ARRAY_CONTAINS(T); \
    int dynamic_array_##T##_contains(DynamicArray_##T *dyn_arr, T elem) \
    { \
        int idx; \
        T *array; \
    \
        array = dyn_arr->array; \
        for (idx = 0; idx < dyn_arr->size; idx++) \
        { \
            if (array[idx] == elem) \
            { \
                return 1; \
            } \
        } \
        return 0; \
    }

/*
 * Clear all elements from the dynamic array and reset the capacity to 10.
 */
#define DEFINE_DYNAMIC_ARRAY_CLEAR(T); \
    void dynamic_array_##T##_clear(DynamicArray_##T *dyn_arr) \
    { \
        dynamic_array_##T##_destruct(dyn_arr); \
        dyn_arr = dynamic_array_##T##_construct(); \
    }

/*
 * Declare signatures of helper methods.
 */
#define DECLARE_DYNAMIC_ARRAY_HELPERS(T); \
    static int dynamic_array_##T##_expand(DynamicArray_##T *dyn_arr);        \
    static int dynamic_array_##T##_contract(DynamicArray_##T *dyn_arr);  \
    static int dynamic_array_##T##_insert_elem(DynamicArray_##T *dyn_arr, T elem, int i); \
    static int dynamic_array_##T##_delete_elem(DynamicArray_##T *dyn_arr, int rem_idx); \
    static void dynamic_array_##T##_recalc_load(DynamicArray_##T *dyn_arr); \

/*
 * Allocate a new array (double the capacity) and copy the elements over.
 *
 * Returns 0 if there is an error allocating the new array. Returns 1 otherwise.
 */
#define DEFINE_DYNAMIC_ARRAY_EXPAND(T); \
    static int dynamic_array_##T##_expand(DynamicArray_##T *dyn_arr) \
    { \
        T *old_array; \
        int new_capacity; \
        int idx; \
    \
        old_array = dyn_arr->array; \
        new_capacity = expansion_factor * (dyn_arr->capacity);  \
        dyn_arr->array = calloc(new_capacity, sizeof(T)); \
        if (dyn_arr->array == NULL) \
        { \
            return 0; \
        } \
        for (idx = 0; idx <= dyn_arr->size; idx++) \
        { \
            dyn_arr->array[idx] = old_array[idx]; \
        } \
        free(old_array); \
    \
        dyn_arr->capacity = new_capacity; \
        dynamic_array_##T##_recalc_load(dyn_arr); \
    \
        return 1; \
    }

/*
 * Allocate a new array (half the capacity) and copy the elements over.
 *
 * Returns 0 if there is an error allocating the new array. Returns 1 otherwise.
 * Note that the array must have, at most, half load or undefined behavior.
 */
#define DEFINE_DYNAMIC_ARRAY_CONTRACT(T); \
static int dynamic_array_##T##_contract(DynamicArray_##T *dyn_arr) \
{                                                                          \
    T *old_array;                                                               \
    int new_capacity;                                                     \
    int idx;                                                                    \
                                                                                \
    old_array = dyn_arr->array;                                                 \
    new_capacity = contraction_factor * dyn_arr->capacity;                                     \
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
    dynamic_array_##T##_recalc_load(dyn_arr); \
                                                                                \
    return 1;                                                                   \
}

/*
 * Insert an element at the i-th index of a dynami array.
 * Does not increment size. Automatically expands if necessary.
 * Helper methods for add, add_at operations.
 * Return 0 if error, positive value if success.
 */
#define DEFINE_DYNAMIC_ARRAY_INSERT_ELEM(T);                                  \
static int dynamic_array_##T##_insert_elem(DynamicArray_##T *dyn_arr, T elem, int i)  \
{                                                                     \
    int idx;                                                               \
    int status;                                                       \
    T *array;                                                               \
                                                                               \
    if (dyn_arr->load == expansion_point)                                         \
    {                                                                    \
        status = dynamic_array_##T##_expand(dyn_arr);                              \
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
    array[idx] = elem;                                                  \
    dyn_arr->size += 1;                                                      \
    dynamic_array_##T##_recalc_load(dyn_arr);                                 \
                                                                              \
    return 1;                                                               \
}

/*
 * Delete the element at the ith index of an array and move all elements down.
 *
 * Automatically contracts the array if it dips below 30% load.
 */
// TODO zero the last element?
// TODO update size?
#define DEFINE_DYNAMIC_ARRAY_DELETE_ELEM(T);                       \
static int dynamic_array_##T##_delete_elem(DynamicArray_##T *dyn_arr, int rem_idx) \
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
    dynamic_array_##T##_recalc_load(dyn_arr);                                 \
                                                                  \
    /* Only half if array has doubled before */                     \
    if (dyn_arr->load <= contraction_point && dyn_arr->capacity > 10)      \
    {                                                              \
        return dynamic_array_##T##_contract(dyn_arr); /* 1 if reallocation successful, 0 if not */ \
    }                                                           \
                                                                  \
    return 1;                                                      \
}

/*
 * Set load = size / capacity.
 */
#define DEFINE_DYNAMIC_ARRAY_RECALC_LOAD(T); \
    static void dynamic_array_##T##_recalc_load(DynamicArray_##T *dyn_arr) \
    { \
        dyn_arr->load = ((float)dyn_arr->size) / ((float)dyn_arr->capacity); \
    }

#endif
















