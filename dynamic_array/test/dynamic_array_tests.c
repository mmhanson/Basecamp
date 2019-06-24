/*
 * Tests for the dynamic array.
 */

#include <stdlib.h> // for random key generation
#include "unity.h"
#include "../dynamic_array.h"

// Capacity after one expansion. Macro so that it can be calculated.
#define EXPANDED_CAPACITY INIT_CAPACITY * EXPANSION_FACTOR 
// Capacity after two expansions.
#define DOUBLE_EXPANDED_CAPACITY EXPANDED_CAPACITY * EXPANSION_FACTOR

DEFINE_DYNAMIC_ARRAY(float);
static void fill_array(DynamicArray_float *darr, int count);
static void expand_array(DynamicArray_float *darr);
static void almost_contract_array(DynamicArray_float *darr);
static int calc_contracted_size(int prev_capacity);


void test_basic_add()
{
    const int elems_to_add = 5;
    DynamicArray_float *darr;
    int idx, key;
    float exp_key;

    darr = dynamic_array_float_construct();

    fill_array(darr, elems_to_add);
    for (idx = 0; idx < elems_to_add; idx++)
    {
        exp_key = (float)idx;
        key = (darr->array)[idx];
        TEST_ASSERT_EQUAL(exp_key, key);
    }

    dynamic_array_float_destruct(darr);
}

void test_basic_add_at()
{
    DynamicArray_float *darr;

    darr = dynamic_array_float_construct();

    fill_array(darr, 5); // partially populate

    // Add some elements 
    dynamic_array_float_add_at(darr, 12.0, 0);
    dynamic_array_float_add_at(darr, 12.2, 2);
    dynamic_array_float_add_at(darr, 12.5, 5);

    // Verify added elements
    TEST_ASSERT_EQUAL_INT(12.0, (darr->array)[0]);
    TEST_ASSERT_EQUAL_INT(12.2, (darr->array)[2]);
    TEST_ASSERT_EQUAL_INT(12.5, (darr->array)[5]);

    dynamic_array_float_destruct(darr);
}

void test_basic_remove()
{
    DynamicArray_float *darr;

    darr = dynamic_array_float_construct();

    fill_array(darr, darr->capacity); // fill up

    // Remove some elements
    dynamic_array_float_remove_at(darr, 0);
    dynamic_array_float_remove_at(darr, 2);
    dynamic_array_float_remove_at(darr, 5);

    // Array should now be: 1, 2, 4, 5, 6, 8, 9, ...
    TEST_ASSERT_EQUAL_INT(1.0, (darr->array)[0]);
    TEST_ASSERT_EQUAL_INT(2.0, (darr->array)[1]);
    TEST_ASSERT_EQUAL_INT(4.0, (darr->array)[2]);
    TEST_ASSERT_EQUAL_INT(5.0, (darr->array)[3]);
    TEST_ASSERT_EQUAL_INT(6.0, (darr->array)[4]);
    TEST_ASSERT_EQUAL_INT(8.0, (darr->array)[5]);
    TEST_ASSERT_EQUAL_INT(9.0, (darr->array)[6]);

    dynamic_array_float_destruct(darr);
}

void test_contains_with_add()
{
    DynamicArray_float *darr;

    darr = dynamic_array_float_construct();

    dynamic_array_float_add(darr, 1.0);
    TEST_ASSERT_TRUE(dynamic_array_float_contains(darr, 1.0));

    dynamic_array_float_destruct(darr);
}

void test_contains_with_add_at()
{
    DynamicArray_float *darr;

    darr = dynamic_array_float_construct();

    dynamic_array_float_add_at(darr, 1.0, 0);
    TEST_ASSERT_TRUE(dynamic_array_float_contains(darr, 1.0));

    dynamic_array_float_destruct(darr);
}

void test_contains_with_remove()
{
    DynamicArray_float *darr;

    darr = dynamic_array_float_construct();

    fill_array(darr, darr->capacity);
    dynamic_array_float_remove(darr, 3.0);
    TEST_ASSERT_FALSE(dynamic_array_float_contains(darr, 3.0));

    dynamic_array_float_destruct(darr);
}

void test_contains_with_remove_at()
{
    DynamicArray_float *darr;

    darr = dynamic_array_float_construct();


    fill_array(darr, darr->capacity);
    dynamic_array_float_remove_at(darr, 4);
    TEST_ASSERT_FALSE(dynamic_array_float_contains(darr, 4.0));

    dynamic_array_float_destruct(darr);
}

void test_expansion_with_add()
{
    DynamicArray_float *darr;

    darr = dynamic_array_float_construct();

    fill_array(darr, darr->capacity);
    dynamic_array_float_add(darr, 42.0);
    TEST_ASSERT_EQUAL_INT(EXPANDED_CAPACITY, darr->capacity);
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY + 1, darr->size);

    dynamic_array_float_destruct(darr);
}

void test_expansion_with_add_at()
{
    DynamicArray_float *darr;

    darr = dynamic_array_float_construct();

    fill_array(darr, darr->capacity);
    dynamic_array_float_add_at(darr, 42.0, 3);
    TEST_ASSERT_EQUAL_INT(EXPANDED_CAPACITY, darr->capacity);
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY + 1, darr->size);

    dynamic_array_float_destruct(darr);
}

void test_contraction_with_remove()
{
    const int CONTRACTED_SIZE = calc_contracted_size(EXPANDED_CAPACITY);
    DynamicArray_float *darr;

    darr = dynamic_array_float_construct();

    expand_array(darr);
    almost_contract_array(darr);
    dynamic_array_float_remove(darr, (darr->array)[3]); // remove elem @ idx 3

    TEST_ASSERT_EQUAL(CONTRACTED_SIZE, darr->size);
    TEST_ASSERT_EQUAL(INIT_CAPACITY, darr->capacity);

    dynamic_array_float_destruct(darr);
}

void test_contraction_with_remove_at()
{
    const int CONTRACTED_SIZE = calc_contracted_size(EXPANDED_CAPACITY);
    DynamicArray_float *darr;

    darr = dynamic_array_float_construct();

    expand_array(darr);
    almost_contract_array(darr);
    dynamic_array_float_remove_at(darr, 3); // remove elem @ idx 3

    TEST_ASSERT_EQUAL(CONTRACTED_SIZE, darr->size);
    TEST_ASSERT_EQUAL(INIT_CAPACITY, darr->capacity);

    dynamic_array_float_destruct(darr);
}

void test_default_values()
{
    DynamicArray_float *darr;

    darr = dynamic_array_float_construct();

    TEST_ASSERT_EQUAL_INT(0, darr->size);
    TEST_ASSERT_EQUAL_INT(INIT_CAPACITY, darr->capacity);
    TEST_ASSERT_EQUAL_INT(0, darr->load);
    TEST_ASSERT_NOT_NULL(darr->array);

    dynamic_array_float_destruct(darr);
}

int main()
{
    UNITY_BEGIN();

    // Populate array and verify the added elements.
    RUN_TEST(test_basic_add);
    // Populate. Add elements with add_at and verify added elements.
    RUN_TEST(test_basic_add_at);
    // Populate. Remove a few elements. Verify all elements.
    RUN_TEST(test_basic_remove);
    // Add an element with add. Verify it is contained.
    RUN_TEST(test_contains_with_add);
    // Add an element with add_at. Verify it is contained.
    RUN_TEST(test_contains_with_add_at);
    // Remove an element with remove. Veify it is not contained.
    RUN_TEST(test_contains_with_remove);
    // Remove an element with remove_at. Veify it is not contained.
    RUN_TEST(test_contains_with_remove_at);
    // Fill the array. Add one more with add. Verify expansion.
    RUN_TEST(test_expansion_with_add);
    // Fill the array. Add one more with add. Verify expansion.
    RUN_TEST(test_expansion_with_add_at);
    // Expand & almost contract array. Remove an elem with remove. Verify.
    RUN_TEST(test_contraction_with_remove);
    // Expand & almost contract array. Remove an elem with remove_at. Verify.
    RUN_TEST(test_contraction_with_remove_at);
    // Construct an array. Verify default values.
    RUN_TEST(test_default_values);

    UNITY_END();
}

// === HELPER METHODS ===

/*
 * Fill an array with 'count' ascending keys starting at 0.
 *
 * For example: passing an empty array and count=10 fills the array with
 * 0.0, 1.0, 2.0, ... , 9.0.
 */
static void fill_array(DynamicArray_float *darr, int count)
{
    float key;
    int counter;

    for (counter = 0; counter < count; counter++)
    {
        key = (float)counter;
        dynamic_array_float_add(darr, key);
    }
}

/*
 * Expand the array by adding one more key than it can hold.
 *
 * The added keys are randomly generated.
 */
static void expand_array(DynamicArray_float *darr)
{
    fill_array(darr, darr->capacity);
    dynamic_array_float_add(darr, rand());
}

/*
 * Remove the first element until removing one more element will contract it.
 */
static void almost_contract_array(DynamicArray_float *darr)
{
    float next_load; // what load will be if one elem is removed

    next_load = (float)(darr->size - 1) / (float)(darr->capacity);
    while (next_load >= CONTRACTION_POINT)
    {
        dynamic_array_float_remove_at(darr, 0);
        next_load = (float)(darr->size - 1) / (float)(darr->capacity);
    }
}

/*
 * Calculate the size of the array just after it contracts from a certain capacity.
 */
static int calc_contracted_size(int prev_capacity)
{
    return (CONTRACTION_POINT * (float)prev_capacity);
}
