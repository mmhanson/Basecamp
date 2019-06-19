#include <stdio.h>
#include "dynamic_array.h"

// define a dynamic array of floats with a parameterized macro
DEFINE_DYNAMIC_ARRAY(float)

int main()
{
    float item = 3.5;

    float_DYNAMIC_ARRAY *dyn_arr = dynamic_array_construct();
    dynamic_array_add(dyn_arr, item);

    printf("(dyn_arr->array)[0]: %f \n", (dyn_arr->array)[0]);
    printf("dyn_arr->size: %d \n", dyn_arr->size);
    printf("dyn_arr->capacity: %d \n", dyn_arr->capacity);
    printf("dyn_arr->load_factor: %f \n", dyn_arr->load_factor);
}
