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
static void half_fill_array(DynamicArray_float *darr);
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

void test_insertion_first()
{
    const int init_cap = 10;
    int size_snp;
    float head_snp;
    DynamicArray_float darr;

    darr_float_init(&darr,
                    malloc(init_cap * sizeof(float)),
                    init_cap);

    darr_float_insert(&darr, 12, 0);

    TEST_ASSERT_EQUAL(1, darr.size);
    TEST_ASSERT_EQUAL(12, darr.array[0]);
}

void test_insertion_new_head()
{
    const int init_cap = 10;
    int size_snp;
    float head_snp;
    DynamicArray_float darr;

    darr_float_init(&darr,
                    malloc(init_cap * sizeof(float)),
                    init_cap);

    half_fill_array(&darr);

    size_snp = darr.size; // Snapshot size.
    head_snp = darr.array[0]; // Snapshot head.
    darr_float_insert(&darr, 12, 0);

    TEST_ASSERT_EQUAL(size_snp + 1, darr.size);
    TEST_ASSERT_EQUAL(12, darr.array[0]);
    TEST_ASSERT_EQUAL(head_snp, darr.array[1]); // Verify old head pushed up.
}

void test_insertion_new_tail()
{
    const int init_cap = 10;
    DynamicArray_float darr;
    int size_snp;
    float tail_snp;

    darr_float_init(&darr,
                    malloc(init_cap * sizeof(float)),
                    init_cap);

    half_fill_array(&darr);

    size_snp = darr.size;
    tail_snp = darr.array[darr.size - 1];
    darr_float_insert(&darr, 12, darr.size);

    TEST_ASSERT_EQUAL(size_snp + 1, darr.size);
    TEST_ASSERT_EQUAL(12, darr.array[darr.size - 1]); // Verify new tail.
    TEST_ASSERT_EQUAL(tail_snp, darr.array[darr.size - 2]); // Verify old tail.
}

void test_insertion_new_midd()
{
    const int insert_idx = 5;
    const int init_cap = 10;
    DynamicArray_float darr;
    float range_snp[3];
    int size_snp;

    darr_float_init(&darr,
                    malloc(init_cap * sizeof(float)),
                    init_cap);

    half_fill_array(&darr);

    // Snapshot of range around insertion
    range_snp[0] = darr.array[insert_idx - 1];
    range_snp[1] = darr.array[insert_idx];
    range_snp[2] = darr.array[insert_idx + 1];
    size_snp = darr.size;
    darr_float_insert(&darr, 12, insert_idx);

    TEST_ASSERT_EQUAL(size_snp + 1, darr.size);
    // Verify array goes: ..., range_snp[0], 12, range_snp[1], range_snp[2], ...
    TEST_ASSERT_EQUAL(range_snp[0], darr.array[insert_idx - 1]);
    TEST_ASSERT_EQUAL(12, darr.array[insert_idx]);
    TEST_ASSERT_EQUAL(range_snp[1], darr.array[insert_idx + 1]);
    TEST_ASSERT_EQUAL(range_snp[2], darr.array[insert_idx + 2]);
}

void test_insertion_full()
{
    const int init_cap = 10;
    DynamicArray_float darr;
    float array_snp[init_cap];
    int size_snp;
    int cap_snp;

    darr_float_init(&darr,
                    malloc(init_cap * sizeof(float)),
                    init_cap);
    fill_array(&darr);

    // Snapshot entire internal array.
    for (int cursor = 0; cursor < init_cap; cursor++)
    {
        array_snp[cursor] = darr.array[cursor];
    }
    size_snp = darr.size;
    cap_snp = darr.capacity;
    darr_float_insert(&darr, 5, 9);

    TEST_ASSERT_EQUAL(size_snp, darr.size);
    TEST_ASSERT_EQUAL(cap_snp, darr.capacity);
    for (int cursor = 0; cursor < darr.capacity; cursor++)
    {
        TEST_ASSERT_EQUAL(array_snp[cursor], darr.array[cursor]);
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

    /// Insertion tests.
    // Insert first elem. Verify.
    RUN_TEST(test_insertion_first);
    // Partly fill array. Insert elem @ first index. Verify.
    RUN_TEST(test_insertion_new_head);
    // Partly fill array. Insert elem @ last index. Verify.
    RUN_TEST(test_insertion_new_tail);
    // Partly fill array. Insert halfway in. Verify.
    RUN_TEST(test_insertion_new_midd);
    // Fill array. Attempt insertion. Verify nothing happened.
    RUN_TEST(test_insertion_full);


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
 * Fill an array to half capacity.
 */
static void half_fill_array(DynamicArray_float *darr)
{
    while (darr->load < 0.5)
    {
        darr_float_insert(darr, 0, 0); // Insert 0 at first index.
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
