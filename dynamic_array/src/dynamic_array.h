/* A dynamic array.
 * See https://en.wikipedia.org/wiki/Dynamic_array for the theory.
 * 
 * This header uses parameterized macros in a convoluted way to create generic
 * structs and functions. If you're not familiar with parameterized macros,
 * you'll have a difficult time understanding whats going on. The usage guide
 * below is meant to be accessible to anyone, but if you're confused then look
 * into parameterized macros and maybe read some articles on making generics in
 * C with them.
 *
 * The reason I made this generic is pretty obvious in my opinion.
 * The reason I chose to use macros to make this generic is to make this header
 * more efficient and easier to use at the cost of making it harder to develop.
 * This could have been done using an array of void-pointers and having the user
 * cast them back and forth, but this approach would necessitate a lot of
 * pointer-following and typecasting and would almost necessitate severe abuse
 * of the heap (e.g. malloc-ing 392 ints). Yes, void pointers would be easier
 * to develop and debugging macros like this is a circus, but using macros means
 * taking those 392 ints scattered throughout the heap and just putting them in
 * a consecutive array. Furthermore, it makes the operations so much easier to
 * use without the constant casting. I think the choice between macros and void
 * pointers is clear.
 *
 * === How to Use ===
 * The first thing you must do is put the macro 'DEFINE_DYNAMIC_ARRAY(T);' at
 * the top of your file. This will declare the dynamic array struct and its
 * operations. The struct will be named 'DynamicArray_T' and the operations will
 * be named 'darr_T_<operation name>(~)'. For example, using the macro
 * 'DEFINE_DYNAMIC_ARRAY(float)' will define the struct 'DynamicArray_float'
 * and some functions prefixed with 'darr_float_...' (see source below for list
 * of operations). Users can declare dynamic arrays holding different types.
 * Their struct and operations are differentiated by the 'T' in their names.
 *
 * After creating a DynamicArray_T, you must initialize it with a new array
 * and set its stats (size, capacity, load). This is all automated with the
 * 'init' operation. After that, to add and remove use the 'insert' and 'remove'
 * operations. See the source below for their precise documentation.
 * 
 * Other than adding and removing, expanding and contracting is the core
 * functionality of the dynamic array. This header doesn't manage memory (ie
 * call `malloc` or `free` at all), so the way it creates this functionality is
 * to 'suggest' an expansion or contraction after an insertion/removal that
 * brings the load of the array to a certain threshold (defined in the macro-
 * constants below). This is done by returning a '1' from the insert/remove
 * operations (see function docs below for more info). Users don't need to
 * expand or contract the array. If an array is full/empty and the user
 * inserts/removes an element, nothing will happen.
 * 
 * However, if users do choose to expand/contract the array, this process is
 * made much easier with the 'realloc' operation. It doesn't allocate the new
 * array, but it allows the user to reallocate the array in one line.
 * 
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
 * consolidated to one line.
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

/* === HELPER FUNCTIONS === */

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
