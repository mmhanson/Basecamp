/*
 * A generic dynamic array implemented from scratch in C89.
 *
 * Define a dynamic array through the macro 'DEFINE_DYNAMIC_ARRAY(T);'. Put this
 * in your code to define a dynamic array called 'DynamicArray_T' that holds
 * type 'T'. For example: 'DEFINE_DYNAMIC_ARRAY(float);' will define a dynamic
 * array called 'DynamicArray_float' that holds floats. This macro will also
 * define all the dynamic array operations as well. These include:
 *   - dynamic_array_T_construct(~)
 *   - dynamic_array_T_destruct(~)
 *   - dynamic_array_T_add(~)
 *   - dynamic_array_T_add_at(~)
 *   - dynamic_array_T_remove(~)
 *   - dynamic_array_T_remove_at(~)
 *   - dynamic_array_T_contains(~)
 * Different types of Dynamic arrays can be defined in the same file. Their
 * types and operations are differentiated by the 'T' in their names.
 * See the macros that define these operations below for their docs.
 * The initial capacity of the array and when it expands/contracts, and by how
 * much, are defined in the constants below.
 *
 * Written by Max Hanson, June 2019.
 */

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdlib.h>

static const int INIT_CAPACITY = 10; /* Initial capacity. */
static const float EXPANSION_POINT = 1.0; /* If load > this, array expands */
/* If load <= this, array contracts */
static const float CONTRACTION_POINT = 0.3;
/* Expanded capacity = this * old capacity */
static const float EXPANSION_FACTOR = 2.0;
/* Contracted capacity = this * old capacity */
static const float CONTRACTION_FACTOR = 0.5;

/*
 * Macro to define a dynamic array of type T and its operations.
 *
 * A type parameter 'T' is valid if, and only if:
 *   - It contains no spaces. Note this means pointers must be typecast.
 */
#define DEFINE_DYNAMIC_ARRAY(T);                                                \
    DEFINE_DYNAMIC_ARRAY_STRUCT(T);                                             \
    DECLARE_DYNAMIC_ARRAY_HELPERS(T);                                           \
    DEFINE_DYNAMIC_ARRAY_CTOR(T);                                               \
    DEFINE_DYNAMIC_ARRAY_DTOR(T);                                               \
    DEFINE_DYNAMIC_ARRAY_ADD(T);                                                \
    DEFINE_DYNAMIC_ARRAY_ADD_AT(T);                                             \
    DEFINE_DYNAMIC_ARRAY_REMOVE(T);                                             \
    DEFINE_DYNAMIC_ARRAY_REMOVE_AT(T);                                          \
    DEFINE_DYNAMIC_ARRAY_CONTAINS(T);                                           \
    DEFINE_DYNAMIC_ARRAY_EXPAND(T);                                             \
    DEFINE_DYNAMIC_ARRAY_CONTRACT(T);                                           \
    DEFINE_DYNAMIC_ARRAY_INSERT_ELEM(T);                                        \
    DEFINE_DYNAMIC_ARRAY_DELETE_ELEM(T);                                        \
    DEFINE_DYNAMIC_ARRAY_RECALC_LOAD(T);

/*
 * A generic dynamic array.
 *
 * A dynamic array is valid if, and only if:
 *   - Its size is how many elements it contains.
 *   - Its capacity is how large the internal array is.
 *   - Its load is its size divided by its capacity.
 *   - All of its elements are consecutive and start at index 0.
 */
#define DEFINE_DYNAMIC_ARRAY_STRUCT(T);                                         \
    typedef struct DynamicArrayTag_##T                                          \
    {                                                                           \
        float load;                                                             \
        int size;                                                               \
        int capacity;                                                           \
        T *array;                                                               \
    } DynamicArray_##T;

/*
 * Create an empty dynamic array on the heap.
 *
 * The initial array capacity is defined in a constant at the top of the file.
 *
 * Every case complexity: BIG-THETA( malloc() ).
 * If:
 *   - The type 'T' supplied to the definition macro is valid.
 * Then:
 *   - A pointer to a valid dynamic array is returned.
 *   - The size and load of the array will be 0.
 *   - The capacity will be initialized according to the constants above.
 *   - The elements of the internal array will be random.
 * Edge cases:
 *   - If there is an error allocating either the internal array or the dynamic
 *     array instance, then null will be returned.
 */
#define DEFINE_DYNAMIC_ARRAY_CTOR(T);                                           \
    DynamicArray_##T *dynamic_array_##T##_construct()                           \
    {                                                                           \
        T *array;                                                               \
                                                                                \
        DynamicArray_##T *dyn_arr = malloc(sizeof(DynamicArray_##T));           \
        array = malloc(INIT_CAPACITY * sizeof(T));                              \
        if (dyn_arr == NULL || array == NULL)                                   \
        {                                                                       \
            return NULL;                                                        \
        }                                                                       \
        dyn_arr->array = array;                                                 \
                                                                                \
        dyn_arr->capacity = INIT_CAPACITY;                                      \
        dyn_arr->size = 0;                                                      \
        dyn_arr->load = 0;                                                      \
    }

/*
 * Free all memory associated with a dynamic array.
 *
 * Every case complexity: BIG-THETA( free() ).
 * If:
 *   - The dynamic array is valid.
 * Then:
 *   - All memory associated with the dynamic array will be deallocated.
 */
#define DEFINE_DYNAMIC_ARRAY_DTOR(T);                                           \
    void dynamic_array_##T##_destruct(DynamicArray_##T *dyn_arr)                \
    {                                                                           \
        free(dyn_arr->array);                                                   \
        free(dyn_arr);                                                          \
    }

/*
 * Add an element to the end of a dynamic array.
 *
 * Every case complexity: O( n + malloc() + free() ). N: size of the array.
 * Amortized complexity: O(1).
 * If:
 *   - The dynamic array is valid.
 * Then:
 *   - The new element is added onto the end of the array and 0 is returned.
 *   - The arrays size will be incremented.
 *   - The arrays load factor will be recalculated.
 *   - If the array is full, then it will be expanded to hold the new element.
 * Edge cases:
 *   - If the array is full and reallocation fails (no more memory), then 1
 *     is returned and the array is not altered.
 */
#define DEFINE_DYNAMIC_ARRAY_ADD(T);                                            \
    int dynamic_array_##T##_add(DynamicArray_##T *dyn_arr, T elem)              \
    {                                                                           \
        return dynamic_array_##T##_insert_elem(dyn_arr, elem, dyn_arr->size);   \
    }

/*
 * Add an element at the i-th index of a dynamic array.
 *
 * Every case complexity: O( n + malloc() + free() ) to expand the array.
 *   N: size of the array.
 * Amortized complexity: O(1).
 * If:
 *   - The dynamic array is valid.
 * Then:
 *   - The element will be the new i-th element of the array.
 *   - The arrays size will be incremented.
 *   - The arrays load factor will be recalculated.
 *   - If the array is full, then it will be expanded to hold the new element.
 * Edge cases:
 *   - If the array is full and reallocation fails (no more memory), then 1
 *     is returned and the array is not altered.
 */
#define DEFINE_DYNAMIC_ARRAY_ADD_AT(T);                                         \
    int dynamic_array_##T##_add_at(DynamicArray_##T *dyn_arr, T elem, int i)    \
    {                                                                           \
        return dynamic_array_##T##_insert_elem(dyn_arr, elem, i);               \
    }

/*
 * Remove the first occurrence of an element in the array.
 *
 * Worst case complexity: O( n + malloc() + free() ) to contract the array.
 *   N: number of elements in the array.
 * Best case complexity: O(1) to remove the last element.
 * Average case complexity: O(n) to remvoe an intermediate element.
 * If:
 *   - The dynamic array is valid.
 * Then:
 *   - The first occurence of the element is removed from the array and all
 *     elements after it are moved one index down.
 *   - The size of the array is decremented.
 *   - The load factor of the array is recalculated.
 *   - If the load is small enough (see constants), then the array is contracted.
 * Edge cases:
 *   - If the array is contracted and there is an error allocating the new array,
 *     then 1 is returned and the original array is not modified.
 */
#define DEFINE_DYNAMIC_ARRAY_REMOVE(T);                                         \
    int dynamic_array_##T##_remove(DynamicArray_##T *dyn_arr, T elem)           \
    {                                                                           \
        int idx;                                                                \
                                                                                \
        for (idx = 0; idx < dyn_arr->size; idx++)                               \
        {                                                                       \
            if ((dyn_arr->array)[idx] == elem)                                  \
            {                                                                   \
                return dynamic_array_##T##_delete_elem(dyn_arr, idx);           \
            }                                                                   \
        }                                                                       \
        return 0;                                                               \
    }

/*
 * Remove the i-th element in the array.
 *
 * Worst case complexity: O( n + malloc() + free() ) to contract the array.
 *   N: number of elements in the array.
 * Best case complexity: O(1) to remove the last element.
 * Average case complexity: O(n) to remvoe an intermediate element.
 * If:
 *   - The dynamic array is valid.
 * Then:
 *   - The i-th element is removed from the array and all elements after it
 *     are moved one index down.
 *   - The size of the array is decremented.
 *   - The load factor of the array is recalculated.
 *   - If the load is small enough (see constants), then the array is contracted.
 * Edge cases:
 *   - If the array is contracted and there is an error allocating the new array,
 *     then 1 is returned and the original array is not modified.
 */
#define DEFINE_DYNAMIC_ARRAY_REMOVE_AT(T);                                      \
    int dynamic_array_##T##_remove_at(DynamicArray_##T *dyn_arr, int i)         \
    {                                                                           \
        return dynamic_array_##T##_delete_elem(dyn_arr, i);                     \
    }

/*
 * Determine if the array contains an element.
 *
 * Every case complexity: O(n).
 * If:
 *   - The dynamic array is valid
 * Then:
 *   - If the array contains the element, then 1 is returned. If it does not,
 *     then 0 is returned.
 */
#define DEFINE_DYNAMIC_ARRAY_CONTAINS(T);                                       \
    int dynamic_array_##T##_contains(DynamicArray_##T *dyn_arr, T elem)         \
    {                                                                           \
        int idx;                                                                \
        T *array;                                                               \
                                                                                \
        array = dyn_arr->array;                                                 \
        for (idx = 0; idx < dyn_arr->size; idx++)                               \
        {                                                                       \
            if (array[idx] == elem)                                             \
            {                                                                   \
                return 1;                                                       \
            }                                                                   \
        }                                                                       \
        return 0;                                                               \
    }

/*
 * Declare signatures of helper methods.
 */
#define DECLARE_DYNAMIC_ARRAY_HELPERS(T);                                       \
    static int dynamic_array_##T##_expand(DynamicArray_##T *dyn_arr);           \
    static int dynamic_array_##T##_contract(DynamicArray_##T *dyn_arr);         \
    static int dynamic_array_##T##_insert_elem(DynamicArray_##T *dyn_arr,       \
                                               T elem, int i);                  \
    static int dynamic_array_##T##_delete_elem(DynamicArray_##T *dyn_arr,       \
                                               int rem_idx);                    \
    static void dynamic_array_##T##_recalc_load(DynamicArray_##T *dyn_arr);     \

/*
 * Expand the array.
 *
 * The capacity of the new array is defined in the EXPANSION_FACTOR constant
 * at the top if this file.
 *
 * Every case complexity: O( n + malloc() + free() ). N: size of array.
 * If:
 *   - The array is valid.
 * Then:
 *   - A new, expanded array is allocated.
 *   - All elements in the dynamic array are copied to this new array.
 *   - The dynamic arrays internal array is swapped with this new array.
 *   - 0 is returned.
 * Edge cases:
 *   - If there is an error allocating the new array, then 1 is returned. and
 *     the old array is not modified.
 */
#define DEFINE_DYNAMIC_ARRAY_EXPAND(T);                                         \
    static int dynamic_array_##T##_expand(DynamicArray_##T *dyn_arr)            \
    {                                                                           \
        T *new_array;                                                           \
        int new_capacity;                                                       \
        int idx;                                                                \
                                                                                \
        /* Allocate new array. */                                               \
        new_capacity = EXPANSION_FACTOR * (dyn_arr->capacity);                  \
        new_array = malloc(new_capacity * sizeof(T));                           \
        if (new_array == NULL)                                                  \
        {                                                                       \
            /* Return and do not alter original array. */                       \
            return 1;                                                           \
        }                                                                       \
                                                                                \
        /* Copy elements over and swap arrays. */                               \
        for (idx = 0; idx <= dyn_arr->size; idx++)                              \
        {                                                                       \
            new_array[idx] = dyn_arr->array[idx];                               \
        }                                                                       \
        free(dyn_arr->array);                                                   \
        dyn_arr->array = new_array;                                             \
                                                                                \
        dyn_arr->capacity = new_capacity;                                       \
        dynamic_array_##T##_recalc_load(dyn_arr);                               \
                                                                                \
        return 0;                                                               \
    }

/*
 * Contract the array.
 *
 * The capacity of the new array is defined in the CONTRACTION_FACTOR constant
 * at the top if this file.
 *
 * Every case complexity: O( n + malloc() + free() ). N: size of array.
 * If:
 *   - The array is valid.
 * Then:
 *   - A new, contracted array is allocated.
 *   - All elements in the dynamic array are copied to this new array.
 *   - The dynamic arrays internal array is swapped with this new array.
 *   - 0 is returned.
 * Edge cases:
 *   - If there is an error allocating the new array, then 1 is returned and the
 *     old array is not modified.
 */
#define DEFINE_DYNAMIC_ARRAY_CONTRACT(T);                                       \
    static int dynamic_array_##T##_contract(DynamicArray_##T *dyn_arr)          \
    {                                                                           \
        T *new_array;                                                           \
        int new_capacity;                                                       \
        int idx;                                                                \
                                                                                \
        /* Allocate new array. */                                               \
        new_capacity = CONTRACTION_FACTOR * dyn_arr->capacity;                  \
        new_array = malloc(new_capacity * sizeof(T));                           \
        if (new_array == NULL)                                                  \
        {                                                                       \
            /* Return error and leave old array unmodified. */                  \
            return 1;                                                           \
        }                                                                       \
                                                                                \
        /* Copy elements over and swap arrays. */                               \
        for (idx = 0; idx <= dyn_arr->size; idx++)                              \
        {                                                                       \
            new_array[idx] = dyn_arr->array[idx];                               \
        }                                                                       \
        free(new_array);                                                        \
        dyn_arr->array = new_array;                                             \
                                                                                \
        dyn_arr->capacity = new_capacity;                                       \
        dynamic_array_##T##_recalc_load(dyn_arr);                               \
                                                                                \
        return 0;                                                               \
    }

/*
 * Insert an element at the i-th index of a dynamic array.
 * Helper methods for add, add_at operations.
 * 
 * Worst case complexity: O(n + malloc() + free() ) to expand the array.
 *   N: size of array.
 * Best case complexity: O(1) to add to the end of the array.
 * If:
 *   - The dynamic array is valid.
 *   - 0 <= i <= dyn_arr->size.
 * Then:
 *   - The element will be the new i-th element in the dynamic array.
 *   - The dynamic arrays size will be incremented.
 *   - The dynamic arrays load factor will be recalculated.
 *   - If the dynamic array is full, then it will be expanded.
 * Edge cases:
 *   - If the dynamic array is full and there is an error expanding it, then 1
 *     is returned.
 */
#define DEFINE_DYNAMIC_ARRAY_INSERT_ELEM(T);                                    \
static int dynamic_array_##T##_insert_elem(DynamicArray_##T *dyn_arr, T elem,   \
                                           int i)                               \
{                                                                               \
    int idx;                                                                    \
    int status;                                                                 \
    T *array;                                                                   \
                                                                                \
    /* Expand if needed. */                                                     \
    if (dyn_arr->load == EXPANSION_POINT)                                       \
    {                                                                           \
        status = dynamic_array_##T##_expand(dyn_arr);                           \
        if (status > 1)                                                         \
        {                                                                       \
            return status; /* pass allocation error code up */                  \
        }                                                                       \
    }                                                                           \
                                                                                \
    /* Move all elements in [i+1..dyn_arr->size) forward one index. */          \
    array = dyn_arr->array;                                                     \
    for (idx = dyn_arr->size; idx > i; idx--)                                   \
    {                                                                           \
        array[idx] = array[idx - 1];                                            \
    }                                                                           \
                                                                                \
    array[idx] = elem;                                                          \
    dyn_arr->size += 1;                                                         \
    dynamic_array_##T##_recalc_load(dyn_arr);                                   \
                                                                                \
    return 0;                                                                   \
}

/*
 * Delete the element at the ith index of a dynamic array.
 * Helper method for the remove, remove_at operations.
 *
 * If:
 *   - The dynamic array is valid.
 *   - 0 <= i < dyn_arr->size.
 * Then:
 *   - The element at the i-th index of the array will be removed.
 *   - All elements higher than the i-th index will be moved an index down.
 *   - The array size will be decremented.
 *   - The array load will be recalculated.
 *   - If the array is sufficiently small after removal (see constants), then
 *     the array will be contracted. The capacity and load will be recalculated.
 *   - 0 is returned.
 * Edge cases:
 *   - If the array is contracted and there is an error allocating a new array,
 *     then 1 is returned.
 */
#define DEFINE_DYNAMIC_ARRAY_DELETE_ELEM(T);                                    \
static int dynamic_array_##T##_delete_elem(DynamicArray_##T *dyn_arr,           \
                                           int i)                               \
{                                                                               \
    int idx;                                                                    \
    T *array;                                                                   \
                                                                                \
    /* Copy every element in [i+1..) back one index. Overwrites array[i] */     \
    array = dyn_arr->array;                                                     \
    for (idx = i + 1; idx < dyn_arr->size; idx++)                               \
    {                                                                           \
        array[idx - 1] = array[idx];                                            \
    }                                                                           \
                                                                                \
    /* Contract if necessary. Only contract if array has expanded before */     \
    if (dyn_arr->load <= CONTRACTION_POINT && dyn_arr->capacity > INIT_CAPACITY)\
    {                                                                           \
        return dynamic_array_##T##_contract(dyn_arr);                           \
    }                                                                           \
                                                                                \
    dyn_arr->size -= 1;                                                         \
    dynamic_array_##T##_recalc_load(dyn_arr);                                   \
                                                                                \
    return 0;                                                                   \
}

/*
 * Set load equal to size divided by capacity.
 *
 * If:
 *   - The dynamic array is valid.
 * Then:
 *   - load will equal size divided by capacity.
 */
#define DEFINE_DYNAMIC_ARRAY_RECALC_LOAD(T);                                    \
    static void dynamic_array_##T##_recalc_load(DynamicArray_##T *dyn_arr)      \
    {                                                                           \
        dyn_arr->load = ((float)dyn_arr->size) / ((float)dyn_arr->capacity);    \
    }

#endif
















