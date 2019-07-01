/*
 * A basic example of using a dynamic array to handle ints.
 *
 * Written by Max Hanson, June 2019.
 * Released into the public domain under CC0. See README.md for more details.
 */

#include <stdio.h>
#include <stdlib.h>
#include "dynamic_array.h"

DEFINE_DYNAMIC_ARRAY(int);
static void print_darr_members(DynamicArray_int *darr);

int main()
{
    const int init_cap = 10;
    DynamicArray_int darr;
    int elem; /* buffer for elems to add/elems just removed */
    int cursor;
    int suggestion; /* suggestion for expansion/contraction */

    darr_int_init(&darr, malloc(init_cap * sizeof(int)), init_cap);

    /* Add random ints to @darr until an expansion is suggested. */
    cursor = 0;
    suggestion = 0;
    while (suggestion != 1)
    {
        elem = rand() % 10;
        suggestion = darr_int_insert(&darr, elem, cursor);

        printf("Inserted '%d' at index '%d'. ", elem, cursor);
        printf("New load: %f\n", darr.load);
        cursor++;
    }
    printf("Expansion suggested after last insert. \n");

    /* Expand to twice the capacity. */
    printf("Expanding array...\n");
    printf("(before expansion) ");
    print_darr_members(&darr);
    free(darr_int_realloc(&darr,
                          malloc(2 * darr.capacity * sizeof(int)),
                          2 * darr.capacity)
        );
    printf("(after expansion) ");
    print_darr_members(&darr);

    /* Remove the end int until a contraction is suggested. */
    /* Notice here that the elements removed from the array are not zeroed out.
       The tail element is removed by simply decrementing the size. */
    cursor = darr.size - 1;
    suggestion = 0;
    while (suggestion != 1)
    {
        elem = darr.array[cursor];
        suggestion = darr_int_remove(&darr, cursor);

        printf("Removed '%d' from index '%d'. \n", elem, cursor);
        cursor--;
    }
    printf("Contraction suggested after last removal. \n");

    /* Contract to half the capacity. */
    printf("Contracting array...\n");
    printf("(before contraction) ");
    print_darr_members(&darr);
    free(darr_int_realloc(&darr,
                          malloc(0.5 * darr.capacity * sizeof(int)),
                          0.5 * darr.capacity)
        );
    printf("(after contraction) ");
    print_darr_members(&darr);

    return 0;
}

/*
 * Print out all memers of the dynamic array.
 */
static void print_darr_members(DynamicArray_int *darr)
{
    int cursor;

    printf("Darr status:\n");

    printf("    size: %d \n", darr->size);
    printf("    capacity: %d \n", darr->capacity);
    printf("    load: %f \n" , darr->load);

    printf("    array (starting at idx. 0): ");
    for (cursor = 0; cursor < darr->capacity; cursor++)
    {
        printf(" %d", darr->array[cursor]);
    }
    printf("\n");
}
