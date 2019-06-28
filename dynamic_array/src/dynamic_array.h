/*
 * A generic dynamic array implemented from scratch in C89.
 *
 * Written by Max Hanson, June 2019.
 * Released into the public domain under CC0. See README.md for more details.
 */

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdlib.h>

/*
 * Constants defining when a dynamic array (suggests) expanding/contracting.
 * See usage guide at top for more details.
 */
static const float EXPANSION_POINT = 1.0; /* load > this -> array expands */
static const float CONTRACTION_POINT = 0.3; /* load < this -> array contracts */
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
#define DEFINE_DYNAMIC_ARRAY(T)                                                \

/*
 * A generic dynamic array.
 *
 * Wrapped in a struct for convenience in handling the load/size/capacity.
 */
#define DEFINE_DYNAMIC_ARRAY_STRUCT(T)                                          \
    typedef struct DynamicArrayTag_##T                                          \
    {                                                                           \
        float load;                                                             \
        int size;                                                               \
        int capacity;                                                           \
        T *array;                                                               \
    } DynamicArray_##T;

/*
 * Initialize a dynamic arra to its default values (zero for all mems).
 */
#define DEFINE_DYNAMIC_ARRAY_INIT(T) \
    static void darr_init(DynamicArray_##T *darr) \
    { \
        darr->load = 0; \
        darr->size = 0; \
        darr->capacity = 0; \
        darr->array = 0; \
    }

/*
 * Copy the elements of a dynamic array to a new array.
 * @new_array must be >= @darr->size in capacity.
 * Swaps @darr's internal array with new_array once done. Updates capacity and load.
 */
#define DEFINE_DYNAMIC_ARRAY_COPY(T) \
static void darr_copy(DynamicArray_##T *darr, T *new_array, int new_array_capacity)
{
    int idx;

    for (idx = 0; idx < darr->size; idx++)
    {
        new_array[idx] = darr[idx]
    }

    darr->array = new_array;
    darr->capacity = new_array_capacity;
    darr->load == __darr_recalc_load(darr->size, darr->capacity);
}

/*
 * Add an element at the i-th index of a dynamic array.
 * @elem will be the new i-th element of the array.
 *
 * Return 2 if expansion needed (cannot add new elem). 1 if expansion suggested
 * Expansion point reached after insertion.
 */
#define DEFINE_DYNAMIC_ARRAY_INSERT(T) \
int darr_##T##_insert(DynamicArray_##T *self, T elem, int i)
{
    int idx;

    if (self->size == self->capapcity)
    {
        return 2; /* expansion needed */
    }

    /* Make hole for the new element and insert it. */
    for (idx = self->size; idx > i; idx--)
    {
        self->array[idx - 1] = self->array[idx];
    }
    self->array[i] = elem;

    self->size++;
    self->load = __darr_recalc_load(self->size, self->capacity);
    if (load >= EXPANSION_POINT)
    {
        return 1; /* expansion suggested */
    }
}


/*
 * Remove the i-th element of a dynamic array.
 *
 * Pair with iteration macro to find and remove an element.
 * Return 1 if contraction suggested. Reached below contraction point.
 */
#define DEFINE_DYNAMIC_ARRAY_REMOVE(T) \
int darr_##T##_remove(DynamicArray_##T *self, int idx)
{
    int cursor;

    /* Copy all in range (idx..self->size) down one index. Overwrites @idx. */
    for (cursor = idx + 1; cursor < self->size; cursor++)
    {
        self->array[cursor - 1] = self->array[cursor];
    }
    self->size--;
    self->load = __darr_recalc_load(darr->size, darr->capacity);
    if (load <= CONTRACTION_POINT)
    {
        return 1; /* contraction suggested */
    }
}

/*
 * Remove the last element in the array.
 * Made a macro to avoid the overhead of an unnecessary function call.
 * Must be a nested macro to put 'T' in the name to fit the rest of the ops.
 *
 * @self must be a pointer to a DynamicArray_T
 */
#define DEFINE_DYNAMIC_ARRAY_POP(T) \
    #define darr_##T##_pop(self) darr_##T##_remove(self, self->size - 1)

/*
 * Append an element to the end of a dynamic array.
 * Made a macro to avoid the overhead of an unnecessary function call.
 * Must be a nested macro to put 'T' in the name to fit the rest of the ops.
 *
 * @elem will be the new last element of the array.
 */
#define DEFINE_DYNAMIC_ARRAY_PUSH(T) \
    #define darr_##T##_push(self, elem) darr_##T##_insert(self, elem, self->size)

/*
 * A simple macro to iterate over a dynamic array after a certain point.
 * Set @start=0 to iterate over whole array.
 *
 * @darr: The DynamicArray_T to iterate over.
 * @idx: int to track where iteration is in array. Needn't be initialized.
 * @start: int of where to start (and include) iteration.
 */
#define DARR_FOR_EACH_AFTER(darr, idx, start) \
    for (idx = start; idx < darr->size; idx++)

/*
 * A simple macro to iterate over a dynamic array between two points.
 *
 * @darr: The DynamicArray_T to iterate over.
 * @idx: int to track where iteration is in array. Needn't be initialized.
 * @start: int of where to start (and include) iteration.
 * @end: int of where to end (not including) iteration.
 */
#define DARR_FOR_EACH_BETWEEN(darr, start, end) \
    for (idx = start; idx < end; idx++)

// === HELPER METHODS ===

/*
 * Recalculate the load of a dynamic array.
 *
 * @size: size of array.
 * @capacity: capapcity of array.
 * @return: the load of the array. size / capacity.
 */
static float __darr_recalc_load(int size, int capacity)
{
    return ((float)self->size) / ((float)self->capacity);
}

#endif
















