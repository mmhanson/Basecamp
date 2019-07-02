/* A dynamic array.
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
 *   this by calling the macro `DEFINE_DYNAMIC_ARRAY(T);` where 'T' is the type
 *   that the dynamic array is to hold (note 'T' must be alphanumerical, pointers must be typecast). This will
 *   define the struct 'DynamicArray_T' and the operations:
 *     - darr_T_init(~)
 *     - darr_T_realloc(~)
 *     - darr_T_insert(~)
 *     - darr_T_remove(~)
 *   The reason the 'T' is included in the struct and operation names is so many
 *   different types of dynamic arrays can be declared and used in the same file.
 *   Calling the macro `DEFINE_DYNAMIC_ARRAY(T);` more than once does nothing.
 *
 * === Declaration Walkthrough ===
 *   To define a dynamic array holding floats, I must put
 *   `DEFINE_DYNAMIC_ARRAY(float);` at the top of my source file. After this, the
     dynamic array type DynamicArray_float is defined as well as the functions:
 *     - darr_float_init(~)
 *     - darr_float_realloc(~)
 *     - darr_float_insert(~)
 *     - darr_float_remove(~)
 *   If I also want a dynamic array holding ints, I can also put
 *   `DEFINE_DYNAMIC_ARRAY(int);` at the top of my source file. After that, the
 *   dynamic array type DynamicArray_int is defined as well as the functions:
 *     - darr_int_init(~)
 *     - darr_int_realloc(~)
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
 *   their return values. The client of this header then can choose to
 *   reallocate. The process of reallocating is made much easier with the
 *   `darr_T_copy(~)` function. Exactly when the insert and remove function will
 *   suggest an expansion or contraction is defined in the macro-constants
 *   below.
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
#define EXPANSION_POINT 1.0 /* if load >= this, then suggest expansion. */
#define CONTRACTION_POINT 0.3 /* if load <= this, then suggest contraction. */

static float __darr_recalc_load(int size, int capacity);

/*
 * Macro to define a dynamic array of type T and its operations.
 * The `#ifndef` is so no errors will show if a dynamic array of the same type
 * is declared twice or more.
 *
 * @T: the type parameter. Must be alphanumerical (pointers must be typecast).
 */
#define DEFINE_DYNAMIC_ARRAY(T); \
    DEFINE_DYNAMIC_ARRAY_STRUCT(T) \
    DEFINE_DYNAMIC_ARRAY_INIT(T) \
    DEFINE_DYNAMIC_ARRAY_REALLOC(T) \
    DEFINE_DYNAMIC_ARRAY_INSERT(T) \
    DEFINE_DYNAMIC_ARRAY_REMOVE(T) \

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
 * Initialize a dynamic array with an internal array.
 * CAUTION: Does not free the old internal array!
 * This function is meant to be called to initialize a brand-new DynamicArray_T
 * struct with a newly allocated internal array. To reallocate the internal
 * array, see the darr_T_realloc function instead.
 *
 * @darr: Pointer to the dynamic array to initialize.
 * @array: Pointer to new allocated array. CAUTION: Must be pointer to array of
 *         type 'T'!
 * @init_cap: Capacity of @array. CAUTION: This value must match capacity of
 *            @array!
 */
#define DEFINE_DYNAMIC_ARRAY_INIT(T) \
    static void darr_##T##_init(DynamicArray_##T *darr, void *array, \
                                int init_cap) \
    { \
        darr->array = array; \
        darr->size = 0; \
        darr->capacity = init_cap; \
        darr->load = __darr_recalc_load(darr->size, darr->capacity); \
    }

/*
 * Reallocate the internal array of a dynamic array.
 * CAUTION: The old internal array of @darr is not deallocated! A pointer to it
 * is returned for the user to deallocate.
 * This function is meant as a convenience to handle the process of expanding
 * a dynamic array. The entire process of allocating a new array, copying the
 * elements, freeing the old array, and tracking the new statistics can be
 * consolidated to one function call.
 *
 * @self: Its internal array's elements will be copied to @new_array, then
 *     @new_array will become its new internal array.
 * @new_array: Pointer to a new array. @self's internal array elements will be
 *     copied into this array in order. CAUTION: must be at least as big in
 *     capacity as @self.size.
 * @new_cap: The capacity of @new_array. How many elements it can hold. Needed
 *     to recalculate members of @self.
 * @return: Pointer to old internal array for deallocation by the user.
 */
#define DEFINE_DYNAMIC_ARRAY_REALLOC(T) \
    static void *darr_##T##_realloc(DynamicArray_##T *self, T *new_array, \
                                int new_cap) \
    { \
        int idx; \
        T *old_array; \
        \
        for (idx = 0; idx < self->size; idx++) \
        { \
            new_array[idx] = (self->array)[idx];  \
        } \
        \
        old_array = self->array; \
        self->array = new_array; \
        self->capacity = new_cap; \
        self->load = __darr_recalc_load(self->size, self->capacity); \
        \
        return old_array; \
    }

/*
 * Add an element into a dynamic array.
 *
 * @self: The dynamic array to insert into. Must have at least one open spot.
 *     Size will be incremented and load recalculated if the add is successful.
 *     If internal array is full, nothing happens.
 * @elem: The element to be added.
 * @idx: The index to add @elem at. @elem will be the new self->array[i].
 *     CAUTION: must be in the range [0..@self.size).
 * @return: 0 if the add was successful. 1 if an expansion is suggested after
 *     the addition. 2 if the @elem could not be added, array is full.
 */
#define DEFINE_DYNAMIC_ARRAY_INSERT(T) \
    static int darr_##T##_insert(DynamicArray_##T *self, T elem, int idx) \
    { \
        int cursor; \
        \
        if (self->size == self->capacity) \
        { \
            return 2; /* expansion needed */ \
        } \
        \
        /* Make hole for the new element and insert it. */ \
        for (cursor = self->size; cursor > idx; cursor--) \
        { \
            self->array[cursor - 1] = self->array[cursor]; \
        } \
        self->array[idx] = elem; \
        \
        self->size++; \
        self->load = __darr_recalc_load(self->size, self->capacity); \
        \
        return (self->load >= EXPANSION_POINT ? 1 : 0); \
    }


/*
 * Remove the i-th element of a dynamic array.
 * This function does not guarantee that extra elements (elements in the array
 * but outside the used part of the array) will be zeroed out. They should be
 * treated as random.
 *
 * @self: The dynamic array to remove from. Size will be decremented and load
 *     will be recalculated. If internal array is empty, nothing happens.
 * @idx: The index the element to remove.
 * @return: 0 if the removal is successful, 1 if a contraction is suggested
 *     after the removal, and 2 if the removal was not successful (array empty).
 */
#define DEFINE_DYNAMIC_ARRAY_REMOVE(T) \
    static int darr_##T##_remove(DynamicArray_##T *self, int idx) \
    { \
        int cursor; \
        \
        if (self->size == 0) \
        { \
            return 2; \
        } \
        /* Copy all in range (idx..self->size) down one. Overwrites @idx. */ \
        for (cursor = idx + 1; cursor < self->size; cursor++) \
        { \
            self->array[cursor - 1] = self->array[cursor]; \
        } \
        \
        self->size--; \
        self->load = __darr_recalc_load(self->size, self->capacity); \
        \
        return (self->load <= CONTRACTION_POINT ? 1 : 0); \
    }

/* === HELPER METHODS === */

/*
 * Recalculate the load of a dynamic array.
 *
 * @size: size of array.
 * @capacity: capapcity of array.
 * @return: the load of the array. size / capacity.
 */
static float __darr_recalc_load(int size, int capacity)
{
    return ((float)size) / ((float)capacity);
}


#endif
