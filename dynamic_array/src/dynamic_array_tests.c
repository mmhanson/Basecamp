/*
 * Unit tests for the dynamic array header.
 *
 * Written by Max Hanson, June 2019.
 * Released into the public domain under CC0. See README.md for more details.
 */

#include "dynamic_array.h"
#include "../../deps/unity/unity.h"
#include <stdlib.h>

DEFINE_DYNAMIC_ARRAY(float)

static void fill_array(DynamicArray_float *darr);
static void double_array(DynamicArray_float *darr);
static int remove_to_suggested_contraction(DynamicArray_float *darr);
static void half_array(DynamicArray_float *darr);


void test_stats_init()
{
    const int init_cap = 10;
    DynamicArray_float darr;

    darr_float_init(&darr,
                    malloc(init_cap * sizeof(float)),
                    init_cap);

    TEST_ASSERT_EQUAL_MESSAGE(0, darr.size,
                              "Size incorrect after initialization.");
    TEST_ASSERT_EQUAL_MESSAGE(init_cap, darr.capacity,
                              "Capacity incorrect after initialization.");
    TEST_ASSERT_EQUAL_MESSAGE(0.0, darr.load,
                              "Load incorrect after initialization.");
}

void test_array_init()
{
    const int init_cap = 10;
    DynamicArray_float darr;

    darr_float_init(&darr,
                    malloc(init_cap * sizeof(float)),
                    init_cap);

    // Insert elements 0..10 in indexes 0..10
    fill_array(&darr);

    // Verify elements 0..10 in indexes 0..10
    for (int cursor = 0; cursor < init_cap; cursor++)
    {
        TEST_ASSERT_EQUAL_MESSAGE(cursor, darr.array[cursor],
            "Incorrect array elements after initialization.");
    }
}

void test_expansion_members()
{
    const int init_cap = 10;
    DynamicArray_float darr;

    darr_float_init(&darr,
                    malloc(init_cap * sizeof(float)),
                    init_cap);
    
    fill_array(&darr);
    double_array(&darr); // Reallocate to 2x the capacity.

    TEST_ASSERT_EQUAL_MESSAGE(init_cap * 2, darr.capacity,
        "Capacity not correct after expansion.");
    TEST_ASSERT_EQUAL_MESSAGE(0.5, darr.load,
        "Load not correct after expansion.");
    TEST_ASSERT_EQUAL_MESSAGE(init_cap, darr.size,
        "Size not correct after expansion.");
}

void test_expansion_copy()
{
    const int init_cap = 10;
    DynamicArray_float darr;

    darr_float_init(&darr,
                    malloc(init_cap * sizeof(float)),
                    init_cap);
    
    fill_array(&darr);
    double_array(&darr); // Reallocate to 2x the capacity.

    // Verify elements copied.
    for (int cursor = 0; cursor < darr.size; cursor++)
    {
        TEST_ASSERT_EQUAL_MESSAGE(cursor, darr.array[cursor],
            "Elements not copied over correctly after reallocation.");
    }
}

void test_expansion_space()
{
    const int init_cap = 10;
    DynamicArray_float darr;

    darr_float_init(&darr,
                    malloc(init_cap * sizeof(float)),
                    init_cap);
    
    fill_array(&darr);
    double_array(&darr); // Reallocate to 2x the capacity.
    fill_array(&darr);

    TEST_ASSERT_EQUAL_MESSAGE(2 * init_cap, darr.size,
        "Error expanding size of array in reallocation.");
    for (int cursor = 0; cursor < darr.size; cursor++)
    {
        TEST_ASSERT_EQUAL_MESSAGE(cursor, darr.array[cursor],
            "Error expanding storage of array in reallocation.");
    }
}

void test_contraction_members()
{
    const int init_cap = 10;
    DynamicArray_float darr;
    int removed;

    darr_float_init(&darr,
                    malloc(init_cap * sizeof(float)),
                    init_cap);

    fill_array(&darr);
    double_array(&darr);
    removed = remove_to_suggested_contraction(&darr);
    half_array(&darr);

    TEST_ASSERT_EQUAL_MESSAGE(init_cap, darr.capacity,
        "Capacity not correct after contraction.");
    TEST_ASSERT_EQUAL_MESSAGE(init_cap - removed, darr.size,
        "Size not correct after contraction.");
    TEST_ASSERT_EQUAL_MESSAGE((float)darr.size / (float)darr.capacity,
        darr.load, "Load not correct after contraction");
}

void test_contraction_copy()
{
    const int init_cap = 10;
    DynamicArray_float darr;
    int removed;

    darr_float_init(&darr,
                    malloc(init_cap * sizeof(float)),
                    init_cap);

    fill_array(&darr);
    double_array(&darr);
    removed = remove_to_suggested_contraction(&darr);
    half_array(&darr);

    for (int cursor = 0; cursor < darr.size; cursor++)
    {
        TEST_ASSERT_EQUAL_MESSAGE(cursor, darr.array[cursor],
            "Elements not copied correctly after contraction.");
    }
}

void test_contraction_space()
{
    const int init_cap = 10;
    DynamicArray_float darr;
    int removed;

    darr_float_init(&darr,
                    malloc(init_cap * sizeof(float)),
                    init_cap);

    fill_array(&darr);
    double_array(&darr);
    removed = remove_to_suggested_contraction(&darr);
    half_array(&darr);
    fill_array(&darr);

    TEST_ASSERT_EQUAL_MESSAGE(init_cap, darr.size,
        "Space not correct after contraction.");
    for (int cursor = 0; cursor < darr.size; cursor++)
    {
        TEST_ASSERT_EQUAL_MESSAGE(cursor, darr.array[cursor],
            "Space not correct after contraction.");
    }
}

int main()
{
    UNITY_BEGIN();

    /// Initialization tests.
    // Init basic array (cap of 10). Verify members (not array).
    RUN_TEST(test_stats_init);
    // Init basic array (cap of 10). Insert 10 elements. Verify elems.
    RUN_TEST(test_array_init);

    /// Expansion tests.
    // Fill array. Expand. Verify darr struct members (cap, load, & size).
    RUN_TEST(test_expansion_members);
    // Fill array. Expand. Verify elements same.
    RUN_TEST(test_expansion_copy);
    // Fill array. Expand. Fill again. Verify filled elems and size.
    RUN_TEST(test_expansion_space);

    /// Contraction tests.
    // Fill array. Expand. Contract. Verify members (cap, size, & load).
    RUN_TEST(test_contraction_members);
    // Fill array. Expand. Contract. Verify elems same.
    RUN_TEST(test_contraction_copy);
    // Fill array. Expand. Contract. Fill again. Verify filled elems & size.
    RUN_TEST(test_contraction_space);



    UNITY_END();
}

// === HELPER METHODS ===

/*
 * Fill array with ascending numbers starting at zero for index 0.
 */
static void fill_array(DynamicArray_float *darr)
{
    for (int cursor = darr->size; cursor < darr->capacity; cursor++)
    {
        darr_float_insert(darr, cursor, cursor);
    }
}

/*
 * Reallocate array with double the space.
 */
static void double_array(DynamicArray_float *darr)
{
    free(darr_float_realloc(darr,
                            malloc(2 * darr->capacity * sizeof(float)),
                            2 * darr->capacity)
        );
}

/*
 * Rellocate array with half the space.
 * CAUTION: make sure array has at most half its capacity.
 */
static void half_array(DynamicArray_float *darr)
{
    free(darr_float_realloc(darr,
                            malloc(0.5 * darr->capacity * sizeof(float)),
                            0.5 * darr->capacity)
        );
}

/*
 * Remove elements from end until remove method suggests a contraction.
 *
 * @return: number of elements removed.
 */
static int remove_to_suggested_contraction(DynamicArray_float *darr)
{
    int removed = 0;
    int suggestion = 0;
    while (suggestion != 1)
    {
        suggestion = darr_float_remove(darr, darr->size - 1);
        removed++;
    }

    return removed;
}
