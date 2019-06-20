/*
 * Tests for the dynamic array.
 */

#include "unity.h"
#include "../dynamic_array.h"

DEFINE_DYNAMIC_ARRAY(float)

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
 * Test that the array doubles and halves as expected.
 */
void test_reallocation()
{
    const int init_cap = 10;
    float_DYNAMIC_ARRAY *dyn_arr;
    int capacity;
    int added;
    int key;

    dyn_arr = dynamic_array_construct();
    TEST_ASSERT_EQUAL(0, dyn_arr->size);
    TEST_ASSERT_EQUAL(init_cap, dyn_arr->capacity);

    // Add one more than the array can hold
    added = 0;
    capacity = dyn_arr->capacity;
    while (added <= capacity)
    {
        key = added + 0.5;
        dynamic_array_add(dyn_arr, key);
        added += 1;
    }
    TEST_ASSERT_EQUAL(init_cap + 1, dyn_arr->size);
    TEST_ASSERT_EQUAL(2 * init_cap, dyn_arr->capacity);

    // Add one more than the reallocated array can hold
    capacity = dyn_arr->capacity;
    while (added <= capacity)
    {
        key = added + 0.5;
        dynamic_array_add(dyn_arr, key);
        added += 1;
    }
    TEST_ASSERT_EQUAL((2 * init_cap) + 1, dyn_arr->size);
    TEST_ASSERT_EQUAL(2 * 2 * init_cap, dyn_arr->capacity);

    // TODO test contraction when array is below 30% load
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
    RUN_TEST(test_reallocation);
    RUN_TEST(test_default_values);
    UNITY_END();
}
