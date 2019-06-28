/*
 * A dynamic array.
 * See https://en.wikipedia.org/wiki/Dynamic_array for the theory.
 * Note that this header applies C macros in a complex way. The usage guide
 * below is meant to be approachable for everyone, but if you're still confused
 * after reading it then look into parameterized macros and maybe look up some
 * articles on making generics in C using them if you're confused.
 *
 * ====================
 * ==== How to Use ====
 * ====================

 * === Declaration ===
 *   To use the struct and functions defined in this header, you must specify the
 *   type of data that the dynamic array is to hold (see the README for why). Do
 *   this by calling the macro `DEFINE_DYNAMIC_ARRAY(T)` where 'T' is the type
 *   that the dynamic array is to hold (note 'T' must be alphanumerical, pointers must be typecast). This will
 *   define the struct 'DynamicArray_T' and the operations:
 *     - darr_T_init(~)
 *     - darr_T_copy(~)
 *     - darr_T_insert(~)
 *     - darr_T_remove(~)
 *   The reason the 'T' is included in the struct and operation names is so many
 *   different types of dynamic arrays can be declared and used in the same file.
 *   Calling the macro `DEFINE_DYNAMIC_ARRAY(T)` more than once does nothing.
 *
 * === Declaration Walkthrough ===
 *   To define a dynamic array holding floats, I must put
 *   `DEFINE_DYNAMIC_ARRAY(float)` at the top of my source file. After this, the
     dynamic array type DynamicArray_float is defined as well as the functions:
 *     - darr_float_init(~)
 *     - darr_float_copy(~)
 *     - darr_float_insert(~)
 *     - darr_float_remove(~)
 *   If I also want a dynamic array holding ints, I can also put
 *   `DEFINE_DYNAMIC_ARRAY(int)` at the top of my source file. After that, the
 *   dynamic array type DynamicArray_int is defined as well as the functions:
 *     - darr_int_init(~)
 *     - darr_int_copy(~)
 *     - darr_int_insert(~)
 *     - darr_int_remove(~)
 *   This way, I can have two types of dynamic arrays (holding floats and ints)
 *   at the same time.
 *
 * === Expansion and Contraction ===
 *   The most important part of a dynamic array is how it expands and contracts.
 *   Since this header doesn't handle memory, dynamic arrays do not
 *   automatically expand and contract. Instead, the insert and remove functions
 *   will indicate that the array is ready for an expansion or contraction with
 *   their return values. The client of this header then can choose to allocate
 *   a new array and copy the values over. Copying the values is made easier
 *   through the `darr_T_copy(~)` function. Exactly when the insert and remove
 *   function will suggest an expansion or contraction is defined in the
 *   constants below.
 *
 * Written by Max Hanson, June 2019.
 * Released into the public domain under CC0. See README.md for more details.
 */

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

/*
 * Constants defining when a dynamic array suggests expanding/contracting.
 * See usage guide at top for more details.
 */
#define EXPANSION_POINT 1.0; /* if load > this, then suggest expansion. */
#define CONTRACTION_POINT 0.3; /* if load < this, then suggest contraction. */

static float __darr_recalc_load(int size, int capacity);


/*
 * Macro to define a dynamic array of type T and its operations.
 * The `#ifndef` is so no errors will show if a dynamic array of the same type
 * is declared twice or more.
 *
 * @T: the type parameter. Must be alphanumerical (pointers must be typecast).
 */
#define DEFINE_DYNAMIC_ARRAY(T) \
    #ifndef __DYNAMIC_ARRAY_##T \
    #define __DYNAMIC_ARRAY_##T
    DEFINE_DYNAMIC_ARRAY_STRUCT(T) \
    DEFINE_DYNAMIC_ARRAY_INIT(T) \
    DEFINE_DYNAMIC_ARRAY_COPY(T) \
    DEFINE_DYNAMIC_ARRAY_INSERT(T) \
    DEFINE_DYNAMIC_ARRAY_REMOVE(T) \
    #endif

/*
 * A dynamic array.
 *
 * @load: How full the array is. load = size / capacity.
 * @size: How many elements of the array are filled.
 * @capacity: How big the array is.
 * @array: Pointer to the array of elements.
 */
#define DEFINE_DYNAMIC_ARRAY_STRUCT(T) \
    typedef struct DynamicArrayTag_##T \
    { \
        float load; \
        int size; \
        int capacity; \
        T *array; \
    } DynamicArray_##T;

/*
 * Initialize a dynamic array to its default values.
 *
 * @darr: Pointer to the dynamic array to initialize. All members will be null.
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
 * The capacity and load of @self are recalculated, size remains the same.
 * Caution! The old internal array of @darr is not deallocated!
 *
 * @self: Its internal array's elements will be copied to @new_array, then
 *     @new_array will become its new internal array.
 * @new_array: Pointer to a new array. @self's internal array elements will be
 *     copied into this array in order. Caution! must be at least as big in
 *     capacity as @self's internal array.
 * @new_cap: The capacity of @new_array. How many elements it can hold. Needed
 *     to recalculate members of @self.
 */
#define DEFINE_DYNAMIC_ARRAY_COPY(T) \
    static void self_copy(DynamicArray_##T *self, T *new_array, int new_cap) \
    { \
        int idx; \
        \
        for (idx = 0; idx < self->size; idx++) \
        { \
            new_array[idx] = self[idx] \
        } \
        \
        self->array = new_array; \
        self->capacity = new_cap; \
        self->load == __self_recalc_load(self->size, self->capacity); \
    }

/*
 * Add an element into a dynamic array.
 *
 * @self: The dynamic array to insert into. Must have at least one open spot.
 *     Size will be incremented and load recalculated if the add is successful.
 * @elem: The element to be added.
 * @idx: The index to add @elem at. @elem will be the new self->array[i].
 * @return: 0 if the add was successful. 1 if an expansion is suggested after
 *     the addition. 2 if the @elem could not be added, array is full.
 */
#define DEFINE_DYNAMIC_ARRAY_INSERT(T) \
    int darr_##T##_insert(DynamicArray_##T *self, T elem, int idx) { \
        int cursor; \
        \
        if (self->size == self->capapcity) \
        { \
            return 2; /* expansion needed */ \
        } \
        \
        /* Make hole for the new element and insert it. */ \
        for (cursor = self->size; cursor > i; cursor--) \
        { \
            self->array[cursor - 1] = self->array[cursor]; \
        } \
        self->array[idx] = elem; \
        \
        self->size++; \
        self->load = __darr_recalc_load(self->size, self->capacity); \
        if (load >= EXPANSION_POINT) { \
            return 1; /* expansion suggested */ \
        } \
        return 0; \
    }


/*
 * Remove the i-th element of a dynamic array.
 *
 * @self: The dynamic array to remove from. Size will be decremented and load
 *     will be recalculated.
 * @idx: The index the element to remove.
 * @return: 1 if a contraction is suggested after the removal. 0 otherwise.
 */
#define DEFINE_DYNAMIC_ARRAY_REMOVE(T) \
    int darr_##T##_remove(DynamicArray_##T *self, int idx) \
    { \
        int cursor; \
        \
        /* Copy all in range (idx..self->size) down one. Overwrites @idx. */ \
        for (cursor = idx + 1; cursor < self->size; cursor++) \
        { \
            self->array[cursor - 1] = self->array[cursor]; \
        } \
        \
        self->size--; \
        self->load = __darr_recalc_load(darr->size, darr->capacity); \
        if (load <= CONTRACTION_POINT) \
        { \
            return 1; /* contraction suggested */ \
        } \
        return 0; \
    }

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
