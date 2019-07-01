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


void test_initialization()
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

int main()
{
    UNITY_BEGIN();

    // Init basic array (cap of 10). Verify members (not array).
    RUN_TEST(test_initialization);

    UNITY_END();
}
