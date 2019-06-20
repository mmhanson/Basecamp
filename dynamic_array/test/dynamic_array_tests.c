/*
 * Tests for the dynamic array.
 */

#include <stdlib.h> // for random key generation
#include "unity.h"
#include "../dynamic_array.h"

DEFINE_DYNAMIC_ARRAY(float)
static void expand_array(float_DYNAMIC_ARRAY *dyn_arr);
static void contract_array(float_DYNAMIC_ARRAY *dyn_arr);
static int calc_contracted_size(int prev_capacity);

/*
 * Test adding to an array without reallocation.
 */
void test_basic_add()
{
    const int init_array_capacity = 10;
    const int elems_to_add = 5;
    float_DYNAMIC_ARRAY *dyn_arr;
    int idx;
    float key;
    float exp_key;
    float exp_load_factor;

    dyn_arr = dynamic_array_construct();
    for (idx = 0; idx < elems_to_add; idx++)
    {
        key = idx + 0.5;
        dynamic_array_add(dyn_arr, key);
    }

    TEST_ASSERT_EQUAL_INT(elems_to_add, dyn_arr->size);
    TEST_ASSERT_EQUAL_INT(init_array_capacity, dyn_arr->capacity);
    exp_load_factor = ((float)dyn_arr->size) / ((float)dyn_arr->capacity);
    TEST_ASSERT_EQUAL_INT(exp_load_factor, dyn_arr->load_factor);
    for (idx = 0; idx < init_array_capacity; idx++)
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
}

/*
 * Test removing from the array without reallocation.
 */
void test_basic_remove()
{
    const int init_array_capacity = 10;
    const int elems_to_add = 5;
    float_DYNAMIC_ARRAY *dyn_arr;
    float *array;
    int idx;
    float key;

    /* Add some elements to the array */
    dyn_arr = dynamic_array_construct();
    for (idx = 0; idx < elems_to_add; idx++)
    {
        key = idx + 0.5;
        dynamic_array_add(dyn_arr, key);
    }

    /* Remove a few elements */
    dynamic_array_remove(dyn_arr, 2);
    dynamic_array_remove(dyn_arr, 0);

    array = dyn_arr->array;
    TEST_ASSERT_EQUAL_INT(1.5, array[0]);
    TEST_ASSERT_EQUAL_INT(3.5, array[1]);
    TEST_ASSERT_EQUAL_INT(4.5, array[2]);
    TEST_ASSERT_EQUAL_INT(3, dyn_arr->size);
}

/*
 * Test that the array expands as expected.
 */
void test_expansion()
{
    const int init_cap = 10; // initial capacity
    const int exp_cap = init_cap * 2; // expanded capacity
    const int dub_exp_cap = init_cap * 4; // double expanded capacity
    float_DYNAMIC_ARRAY *dyn_arr;

    dyn_arr = dynamic_array_construct();

    expand_array(dyn_arr);
    TEST_ASSERT_EQUAL(init_cap + 1, dyn_arr->size);
    TEST_ASSERT_EQUAL(exp_cap, dyn_arr->capacity);

    expand_array(dyn_arr);
    TEST_ASSERT_EQUAL(exp_cap + 1, dyn_arr->size);
    TEST_ASSERT_EQUAL(dub_exp_cap, dyn_arr->capacity);
}

/*
 * Test that the array contracts as expected.
 */
void test_contraction()
{
    const int init_cap = 10; // initial capacity
    const int exp_expanded_cap = init_cap * 2; // expected expanded capacity
    const int exp_dub_expanded_cap = init_cap * 4; // double expanded capacity
    const int exp_contd_size = calc_contracted_size(exp_expanded_cap);
    const int exp_dub_contd_size = calc_contracted_size(exp_dub_expanded_cap);
    float_DYNAMIC_ARRAY *dyn_arr;

    dyn_arr = dynamic_array_construct();

    expand_array(dyn_arr);
    contract_array(dyn_arr);
    TEST_ASSERT_EQUAL(exp_contd_size, dyn_arr->size);
    TEST_ASSERT_EQUAL(init_cap, dyn_arr->capacity);

    expand_array(dyn_arr);
    expand_array(dyn_arr);
    contract_array(dyn_arr);
    TEST_ASSERT_EQUAL(exp_dub_contd_size, dyn_arr->size);
    TEST_ASSERT_EQUAL(exp_expanded_cap, dyn_arr->capacity);
}

/*
 * Test the default values of a dynamic arrays members after construction.
 */
void test_default_values()
{
    const int array_init_capacity = 10;
    float_DYNAMIC_ARRAY *dyn_arr;
    int idx;

    dyn_arr = dynamic_array_construct();

    TEST_ASSERT_EQUAL_INT(0, dyn_arr->size);
    TEST_ASSERT_EQUAL_INT(10, dyn_arr->capacity);
    TEST_ASSERT_EQUAL_INT(0, dyn_arr->load_factor);
    TEST_ASSERT_NOT_NULL(dyn_arr->array);
    for (idx = 0; idx < array_init_capacity; idx++)
    {
        TEST_ASSERT_EQUAL(0.0, dyn_arr->array[idx]);
    }
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_basic_add);
    RUN_TEST(test_basic_remove);
    RUN_TEST(test_expansion);
    RUN_TEST(test_contraction);
    RUN_TEST(test_default_values);
    UNITY_END();
}

// === HELPER METHODS ===

/*
 * Expand the array by adding one more key than it can hold.
 *
 * The added keys are randomly generated.
 */
static void expand_array(float_DYNAMIC_ARRAY *dyn_arr)
{
    // Fill the array up.
    while (dyn_arr->load_factor != 1)
    {
        dynamic_array_add(dyn_arr, rand());
    }
    // Add one more.
    dynamic_array_add(dyn_arr, rand());
}

/*
 * Contract the array by removing the first key until it halves.
 */
static void contract_array(float_DYNAMIC_ARRAY *dyn_arr)
{
    float cap_snp; // snapshot of the capacity

    cap_snp = dyn_arr->capacity;
    // Remove first elem until capacity changes (contracts).
    while(dyn_arr->capacity == cap_snp)
    {
        dynamic_array_remove_at(dyn_arr, 0);
    }
}

/*
 * Calculate the size of the array just after it contracts from a certain size.
 */
static int calc_contracted_size(int prev_capacity)
{
    float load_factor;
    int size;

    size = prev_capacity;
    load_factor = ((float)size) / ((float)prev_capacity);
    while (load_factor > 0.3000000000000) // account for precision error in load
    {
        size -= 1;
        load_factor = ((float)size) / ((float)prev_capacity);
    }

    return size;
}












