/*
 * An example of how to use the linked list header.
 * Written in C89 to test compiling linked list under C89 standards.
 * In this example, a list of car parts is created and iterated over.
 *
 * Written by Max Hanson, June 2019.
 * Licensed under MIT. See LICENSE.txt for more details.
 */

#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct CarPartTag CarPart;
static void car_part_init(CarPart *part, const char *name, int number,
                          int stock);


struct CarPartTag
{
    char name[30]; /* part name */
    int number; /* part number */
    int stock; /* how many are in stock */
    ListNode node; /* how struct instances are linked together */
};

int main(int argc, char *argv[])
{
    CarPart *part_a, *part_b, *part_c;
    CarPart *cursor_container;
    ListNode *cursor;

    /* Initialize all car parts. */
    part_a = malloc(sizeof(CarPart));
    part_b = malloc(sizeof(CarPart));
    part_c = malloc(sizeof(CarPart));
    car_part_init(part_a, "radiator hose", 10035, 4);
    car_part_init(part_b, "lugnut", 10036, 12);
    car_part_init(part_c, "carburator", 10037, 3);

    /* Create the list with @part_a as its only node. */
    list_init(&part_a->node, part_a);
    /* Add @part_b after @part_a. */
    list_add_after(&part_a->node, &part_b->node, part_b);
    /* Add @part_c after @part_b. */
    list_add_after(&part_b->node, &part_c->node, part_c);
    /* Now list goes: part_a<->part_b<->part_c. */

    /* Iterate through the list from part_a to part_c and print the names. */
    printf("Iterating over entire list...\n");
    LIST_FOR_EACH_AFTER(&part_a->node, cursor)
    {
        cursor_container = (CarPart*)cursor->container;
        printf("    "); /* indentation */
        printf("Part: {name: %s, number: %d, stock %d}\n",
               cursor_container->name,
               cursor_container->number,
               cursor_container->stock);
    }

    /* Iterate only between part_a (including) and part_c (discluding). */
    printf("Iterating from part_a (including) to part_c (discluing)...\n");
    LIST_FOR_EACH_BETWEEN(&part_a->node, &part_c->node, cursor)
    {
        cursor_container = (CarPart*)cursor->container;
        printf("    "); /* indentation */
        printf("Part: {name: %s, number: %d, stock %d}\n",
               cursor_container->name,
               cursor_container->number,
               cursor_container->stock);
    }

    free(part_a);
    free(part_b);
    free(part_c);
}

/*
 * Initialize a car part to certain values.
 *
 * @part: Car part to initialize.
 * @name: the name of the car part. Must be <30 chars!
 * @number: the part number.
 * @stock: how many are in stock.
 */
static void car_part_init(CarPart *part, const char *name, int number,
                          int stock)
{
    int idx;

    /* Manually copy name to part->name. */
    for (idx = 0; name[idx] != '\0'; idx++)
    {
        part->name[idx] = name[idx];
    }
    part->name[idx + 1] = '\0';
    
    part->number = number;
    part->stock = stock;
}
