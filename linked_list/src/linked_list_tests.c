/*
 * Some basic tests for the linked list header.
 *
 * Written by Max Hanson, June 2019.
 * Licensed under MIT. See LICENSE.txt for more details.
 */
// TODO add messages to assertions

#include "linked_list.h"
#include "../../deps/unity/unity.h"
#include <stdlib.h>

/*
 * Dummy struct to test linking together.
 */
typedef struct ContainerStructTag
{
    int dummy_mem;
    ListNode node;
} ContainerStruct;

void test_default_values()
{
    ContainerStruct *cont;

    cont = malloc(sizeof(ContainerStruct));
    cont->dummy_mem = 0;
    list_init(&cont->node, cont);

    TEST_ASSERT_EQUAL(cont, cont->node.container);
    TEST_ASSERT_EQUAL(0, cont->node.next);
    TEST_ASSERT_EQUAL(0, cont->node.prev);

    free(cont);
}

void test_add_after_list_of_one()
{
    ContainerStruct *head, *to_add;

    head = malloc(sizeof(ContainerStruct));
    to_add = malloc(sizeof(ContainerStruct));
    head->dummy_mem = 1;
    to_add->dummy_mem = 2;
    list_init(&head->node, head);

    list_add_after(&head->node, &to_add->node, to_add);

    // Test nodes link together.
    TEST_ASSERT_EQUAL(&to_add->node, head->node.next);
    TEST_ASSERT_EQUAL(&head->node, to_add->node.prev);

    // Test nodes only link together.
    TEST_ASSERT_EQUAL(0, head->node.prev);
    TEST_ASSERT_EQUAL(0, to_add->node.next);

    // Test container member unmodified.
    TEST_ASSERT_EQUAL(1, head->dummy_mem);
    TEST_ASSERT_EQUAL(2, to_add->dummy_mem);
}


int main()
{
    UNITY_BEGIN();

    // Verify default values after initializing a list of one.
    RUN_TEST(test_default_values);
    // Test adding an element after head of list of one.
    RUN_TEST(test_add_after_list_of_one);
}
