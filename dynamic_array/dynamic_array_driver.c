/*
 * A basic client for the dynamic array to test compiling it in C89.
 *
 * Written by Max Hanson, June 2019.
 * Licensed under MIT. See LICENSE.txt for more details.
 */

#include <stdio.h>
#include "dynamic_array.h"

DEFINE_DYNAMIC_ARRAY(float)

int main()
{
    DynamicArray_float *darr;

    darr = dynamic_array_float_construct();
    printf("Constructed dynamic array.\n");

    dynamic_array_float_add_at(darr, 1.5, 1);
    printf("Added '1.5' to darr[1]. \n");

    dynamic_array_float_destruct(darr);
    printf("Destructed dynamic array.\n");
}
