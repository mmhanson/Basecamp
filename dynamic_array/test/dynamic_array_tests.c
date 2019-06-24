/*
 * Tests for the dynamic array.
 */

#include <stdlib.h> // for random key generation
#include "unity.h"
#include "../dynamic_array.h"

DEFINE_DYNAMIC_ARRAY(float);
static void fill_array(DynamicArray_float *dyn_arr);
static void expand_array(DynamicArray_float *dyn_arr);
static void contract_array(DynamicArray_float *dyn_arr);
static int calc_contracted_size(int prev_capacity);

// Capacity after one expansion. Macro so that it can be calculated.
#define EXPANDED_CAPACITY INIT_CAPACITY * EXPANSION_FACTOR 
// Capacity after two expansions.
#define DOUBLE_EXPANDED_CAPACITY EXPANDED_CAPACITY * EXPANSION_FACTOR

/*
 * Test adding to the end of an array without reallocation.
 */
void test_basic_add()
{
    const int elems_to_add = 5;
    DynamicArray_float *dyn_arr;
    int idx;
    float key;
    float exp_key;
    float exp_load;

    dyn_arr = dynamic_array_float_construct();
    for (idx = 0; idx < elems_to_add; idx++)
    {
        key = idx + 0.5;
        dynamic_array_float_add(dyn_arr, key);
    }

    TEST_ASSERT_EQUAL_INT(elems_to_add, dyn_arr->size);
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, dyn_arr->capacity);
    exp_load = ((float)dyn_arr->size) / ((float)dyn_arr->capacity);
    TEST_ASSERT_EQUAL_INT(exp_load, dyn_arr->load);
    for (idx = 0; idx < INIT_CAPACITY; idx++)
    {
        if (idx < elems_to_add)
        {
            exp_key = idx + 0.5;
            TEST_ASSERT_EQUAL_INT(exp_key, (dyn_arr->array)[idx]);
        }
        else
        {
            exp_key = 0.0;
            TEST_ASSERT_EQUAL_INT(exp_key, (dyn_arr->array)[idx]);
        }
    }

    dynamic_array_float_destruct(dyn_arr);
}

/*
 * Test adding to an array without reallocation.
 */
void test_basic_add_at()
{
    DynamicArray_float *dyn_arr;
    float *array;
    int idx;

    dyn_arr = dynamic_array_float_construct();

    /* Add 5 keys to the end */
    dynamic_array_float_add(dyn_arr, 1.5);
    dynamic_array_float_add(dyn_arr, 1.5);
    dynamic_array_float_add(dyn_arr, 1.5);
    dynamic_array_float_add(dyn_arr, 1.5);
    dynamic_array_float_add(dyn_arr, 1.5);

    /* Add 5 keys intermittently */
    dynamic_array_float_add_at(dyn_arr, 2.5, 0);
    dynamic_array_float_add_at(dyn_arr, 2.5, 3);
    dynamic_array_float_add_at(dyn_arr, 2.5, 4);
    dynamic_array_float_add_at(dyn_arr, 2.5, 6);
    dynamic_array_float_add_at(dyn_arr, 2.5, 7);

    /* Verify the values of the array */
    array = dyn_arr->array;
    TEST_ASSERT_EQUAL_INT(2.5, array[0]);
    TEST_ASSERT_EQUAL_INT(1.5, array[1]);
    TEST_ASSERT_EQUAL_INT(1.5, array[2]);
    TEST_ASSERT_EQUAL_INT(2.5, array[3]);
    TEST_ASSERT_EQUAL_INT(2.5, array[4]);
    TEST_ASSERT_EQUAL_INT(1.5, array[5]);
    TEST_ASSERT_EQUAL_INT(2.5, array[6]);
    TEST_ASSERT_EQUAL_INT(2.5, array[7]);
    TEST_ASSERT_EQUAL_INT(1.5, array[8]);
    TEST_ASSERT_EQUAL_INT(1.5, array[9]);

    /* Verify that array didn't expand */
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, dyn_arr->capacity);
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, dyn_arr->size); // full array

    dynamic_array_float_destruct(dyn_arr);
}

/*
 * Test removing from the array without reallocation.
 */
void test_basic_remove()
{
    const int elems_to_add = 5;
    DynamicArray_float *dyn_arr;
    float *array;
    int idx;
    float key;

    /* Add some elements to the array */
    dyn_arr = dynamic_array_float_construct();
    for (idx = 0; idx < elems_to_add; idx++)
    {
        key = idx + 0.5;
        dynamic_array_float_add(dyn_arr, key);
    }

    /* Remove a few elements */
    dynamic_array_float_remove_at(dyn_arr, 2);
    dynamic_array_float_remove_at(dyn_arr, 0);

    array = dyn_arr->array;
    TEST_ASSERT_EQUAL_INT(1.5, array[0]);
    TEST_ASSERT_EQUAL_INT(3.5, array[1]);
    TEST_ASSERT_EQUAL_INT(4.5, array[2]);
    TEST_ASSERT_EQUAL_INT(3, dyn_arr->size);

    dynamic_array_float_destruct(dyn_arr);
}

/*
 * Test the contains operation with array expansion.
 */
void test_contains()
{
    DynamicArray_float *dyn_arr;
    float *array;

    dyn_arr = dynamic_array_float_construct();
    array = dyn_arr->array;

    // Test that the array contains elements just added.
    dynamic_array_float_add(dyn_arr, 1.5);
    dynamic_array_float_add(dyn_arr, 2.5);
    dynamic_array_float_add(dyn_arr, 3.5);
    dynamic_array_float_add(dyn_arr, 4.5);
    TEST_ASSERT_TRUE(dynamic_array_float_contains(dyn_arr, 1.5));
    TEST_ASSERT_TRUE(dynamic_array_float_contains(dyn_arr, 2.5));
    TEST_ASSERT_TRUE(dynamic_array_float_contains(dyn_arr, 3.5));
    TEST_ASSERT_TRUE(dynamic_array_float_contains(dyn_arr, 4.5));

    // Test that the array contains added elements after expansion.
    expand_array(dyn_arr);
    TEST_ASSERT_TRUE(dynamic_array_float_contains(dyn_arr, 1.5));
    TEST_ASSERT_TRUE(dynamic_array_float_contains(dyn_arr, 2.5));
    TEST_ASSERT_TRUE(dynamic_array_float_contains(dyn_arr, 3.5));
    TEST_ASSERT_TRUE(dynamic_array_float_contains(dyn_arr, 4.5));

    // Add some more and test that it contains them.
    dynamic_array_float_add(dyn_arr, 5.5);
    dynamic_array_float_add(dyn_arr, 6.5);
    TEST_ASSERT_TRUE(dynamic_array_float_contains(dyn_arr, 5.5));
    TEST_ASSERT_TRUE(dynamic_array_float_contains(dyn_arr, 6.5));

    // Test that array contains elements added with add_at.
    dynamic_array_float_destruct(dyn_arr);
    dyn_arr = dynamic_array_float_construct(); // new array
    array = dyn_arr->array;
    dynamic_array_float_add_at(dyn_arr, 1.5, 3);
    dynamic_array_float_add_at(dyn_arr, 2.5, 3); // overlaping index
    dynamic_array_float_add_at(dyn_arr, 3.5, 5);
    TEST_ASSERT_TRUE(dynamic_array_float_contains(dyn_arr, 1.5));
    TEST_ASSERT_TRUE(dynamic_array_float_contains(dyn_arr, 2.5));
    TEST_ASSERT_TRUE(dynamic_array_float_contains(dyn_arr, 3.5));

    // Test that array still contains elements after expansion with add_at.
    fill_array(dyn_arr);
    dynamic_array_float_add_at(dyn_arr, 9.9, 0);
    TEST_ASSERT_TRUE(dynamic_array_float_contains(dyn_arr, 1.5));
    TEST_ASSERT_TRUE(dynamic_array_float_contains(dyn_arr, 2.5));
    TEST_ASSERT_TRUE(dynamic_array_float_contains(dyn_arr, 3.5));
    TEST_ASSERT_TRUE(dynamic_array_float_contains(dyn_arr, 9.9));

    // Test that elements removed with remove operation are not contained.
    dynamic_array_float_destruct(dyn_arr);
    dyn_arr = dynamic_array_float_construct(); // new array
    array = dyn_arr->array;
    dynamic_array_float_add_at(dyn_arr, 1.5, 3);
    dynamic_array_float_add(dyn_arr, 2.5);
    dynamic_array_float_remove(dyn_arr, 2.5);
    dynamic_array_float_remove(dyn_arr, 1.5);

    // Test that elements removed with remove_at operation are not contained.
    dynamic_array_float_destruct(dyn_arr);
    dyn_arr = dynamic_array_float_construct(); // new array
    array = dyn_arr->array;
    dynamic_array_float_add(dyn_arr, 2.5);
    dynamic_array_float_add_at(dyn_arr, 1.5, 0);
    dynamic_array_float_remove_at(dyn_arr, 0);
    dynamic_array_float_remove_at(dyn_arr, 0);

}

/*
 * Test that the array expands as expected.
 */
void test_expansion()
{
    DynamicArray_float *dyn_arr;

    // Verify that the array expands when over capacity twice.
    dyn_arr = dynamic_array_float_construct();
    expand_array(dyn_arr);
    TEST_ASSERT_EQUAL(INIT_CAPACITY + 1, dyn_arr->size);
    TEST_ASSERT_EQUAL(EXPANDED_CAPACITY, dyn_arr->capacity);
    expand_array(dyn_arr);
    TEST_ASSERT_EQUAL(EXPANDED_CAPACITY + 1, dyn_arr->size);
    TEST_ASSERT_EQUAL(DOUBLE_EXPANDED_CAPACITY, dyn_arr->capacity);

    // Verify that adding to a full array with the add operation expands it.
    dynamic_array_float_destruct(dyn_arr);
    dyn_arr = dynamic_array_float_construct(); // new array
    fill_array(dyn_arr);
    dynamic_array_float_add(dyn_arr, 42.5);
    TEST_ASSERT_EQUAL(INIT_CAPACITY + 1, dyn_arr->size);
    TEST_ASSERT_EQUAL(EXPANDED_CAPACITY, dyn_arr->capacity);

    // Verify that adding to a full array with the add_at operation expands it.
    dynamic_array_float_destruct(dyn_arr);
    dyn_arr = dynamic_array_float_construct(); // new array
    fill_array(dyn_arr);
    dynamic_array_float_add_at(dyn_arr, 42.5, 5);
    TEST_ASSERT_EQUAL(INIT_CAPACITY + 1, dyn_arr->size);
    TEST_ASSERT_EQUAL(EXPANDED_CAPACITY, dyn_arr->capacity);

    dynamic_array_float_destruct(dyn_arr);
}

/*
 * Test that the array contracts as expected.
 */
void test_contraction()
{
    // Expected sizes of array after contracting from one and two expansions
    const int CONTRACTED_SIZE = calc_contracted_size(EXPANDED_CAPACITY);
    const int DOUBLE_CONTRACTED_SIZE =
        calc_contracted_size(DOUBLE_EXPANDED_CAPACITY);
    DynamicArray_float *dyn_arr;

    dyn_arr = dynamic_array_float_construct();

    expand_array(dyn_arr);
    contract_array(dyn_arr);
    TEST_ASSERT_EQUAL(CONTRACTED_SIZE, dyn_arr->size);
    TEST_ASSERT_EQUAL(INIT_CAPACITY, dyn_arr->capacity);

    expand_array(dyn_arr);
    expand_array(dyn_arr);
    contract_array(dyn_arr);
    TEST_ASSERT_EQUAL(DOUBLE_CONTRACTED_SIZE, dyn_arr->size);
    TEST_ASSERT_EQUAL(EXPANDED_CAPACITY, dyn_arr->capacity);

    dynamic_array_float_destruct(dyn_arr);
}

/*
 * Test the default values of a dynamic arrays members after construction.
 */
void test_default_values()
{
    DynamicArray_float *dyn_arr;
    int idx;

    dyn_arr = dynamic_array_float_construct();

    TEST_ASSERT_EQUAL_INT(0, dyn_arr->size);
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, dyn_arr->capacity);
    TEST_ASSERT_EQUAL_INT(0, dyn_arr->load);
    TEST_ASSERT_NOT_NULL(dyn_arr->array);

    dynamic_array_float_destruct(dyn_arr);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_basic_add);
    RUN_TEST(test_basic_add_at);
    RUN_TEST(test_basic_remove);
    RUN_TEST(test_contains);
    RUN_TEST(test_expansion);
    RUN_TEST(test_contraction);
    RUN_TEST(test_default_values);
    UNITY_END();
}

// === HELPER METHODS ===

/*
 * Fill the array with random keys until the load is the expansion point.
 */
static void fill_array(DynamicArray_float *dyn_arr)
{
    // Fill to point of expansion
    while (dyn_arr->load != EXPANSION_POINT)
    {
        dynamic_array_float_add(dyn_arr, rand());
    }
}

/*
 * Expand the array by adding one more key than it can hold.
 *
 * The added keys are randomly generated.
 */
static void expand_array(DynamicArray_float *dyn_arr)
{
    fill_array(dyn_arr);
    dynamic_array_float_add(dyn_arr, rand());
}

/*
 * Contract the array by removing the first key until it halves.
 */
static void contract_array(DynamicArray_float *dyn_arr)
{
    float capacity_snp; // snapshot of the capacity

    capacity_snp = dyn_arr->capacity;
    // Remove first elem until capacity contracts.
    while(dyn_arr->capacity == capacity_snp)
    {
        dynamic_array_float_remove_at(dyn_arr, 0);
    }
}

/*
 * Calculate the size of the array just after it contracts from a certain capacity.
 */
static int calc_contracted_size(int prev_capacity)
{
    return (CONTRACTION_POINT * (float)prev_capacity);
}
