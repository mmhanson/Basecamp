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

void test_add_after_last_node()
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

    free(head);
    free(to_add);
}

void test_add_after_mid_node()
{
    ContainerStruct *head, *mid, *tail, *to_add;

    head = malloc(sizeof(ContainerStruct));
    mid = malloc(sizeof(ContainerStruct));
    tail = malloc(sizeof(ContainerStruct));
    to_add = malloc(sizeof(ContainerStruct));
    list_init(&head->node, head);
    list_add_after(&head->node, &mid->node, mid);
    list_add_after(&mid->node, &tail->node, tail);

    list_add_after(&mid->node, &to_add->node, to_add);
    // List should be: head <-> mid <-> to_add <-> tail

    // Assert head points to correct nodes.
    TEST_ASSERT_EQUAL(0, head->node.prev);
    TEST_ASSERT_EQUAL(&mid->node, head->node.next);

    // Assert mid points to correct nodes.
    TEST_ASSERT_EQUAL(&head->node, mid->node.prev);
    TEST_ASSERT_EQUAL(&to_add->node, mid->node.next);

    // Assert to_add points to correct nodes.
    TEST_ASSERT_EQUAL(&mid->node, to_add->node.prev);
    TEST_ASSERT_EQUAL(&tail->node, to_add->node.next);

    // Assert tail points to correct nodes.
    TEST_ASSERT_EQUAL(&to_add->node, tail->node.prev);
    TEST_ASSERT_EQUAL(0, tail->node.next);

    free(head);
    free(mid);
    free(to_add);
    free(tail);
}

void test_add_before_first_node()
{
    ContainerStruct *head, *to_add;

    head = malloc(sizeof(ContainerStruct));
    to_add = malloc(sizeof(ContainerStruct));
    head->dummy_mem = 1;
    to_add->dummy_mem = 2;
    list_init(&head->node, head);

    list_add_before(&head->node, &to_add->node, to_add);
    // List should no be: to_add <-> head

    // Test nodes link together.
    TEST_ASSERT_EQUAL(&to_add->node, head->node.prev);
    TEST_ASSERT_EQUAL(&head->node, to_add->node.next);

    // Test nodes only link together.
    TEST_ASSERT_EQUAL(0, head->node.next);
    TEST_ASSERT_EQUAL(0, to_add->node.prev);

    // Test container member unmodified.
    TEST_ASSERT_EQUAL(1, head->dummy_mem);
    TEST_ASSERT_EQUAL(2, to_add->dummy_mem);

    free(head);
    free(to_add);
}

void test_add_before_mid_node()
{
    ContainerStruct *head, *mid, *tail, *to_add;

    head = malloc(sizeof(ContainerStruct));
    mid = malloc(sizeof(ContainerStruct));
    tail = malloc(sizeof(ContainerStruct));
    to_add = malloc(sizeof(ContainerStruct));
    list_init(&head->node, head);
    list_add_after(&head->node, &mid->node, mid);
    list_add_after(&mid->node, &tail->node, tail);

    list_add_before(&tail->node, &to_add->node, to_add);
    // List should be: head <-> mid <-> to_add <-> tail

    // Assert head points to correct nodes.
    TEST_ASSERT_EQUAL(0, head->node.prev);
    TEST_ASSERT_EQUAL(&mid->node, head->node.next);

    // Assert mid points to correct nodes.
    TEST_ASSERT_EQUAL(&head->node, mid->node.prev);
    TEST_ASSERT_EQUAL(&to_add->node, mid->node.next);

    // Assert to_add points to correct nodes.
    TEST_ASSERT_EQUAL(&mid->node, to_add->node.prev);
    TEST_ASSERT_EQUAL(&tail->node, to_add->node.next);

    // Assert tail points to correct nodes.
    TEST_ASSERT_EQUAL(&to_add->node, tail->node.prev);
    TEST_ASSERT_EQUAL(0, tail->node.next);

    free(head);
    free(mid);
    free(to_add);
    free(tail);
}

int main()
{
    UNITY_BEGIN();

    // Verify default values after initializing a list of one.
    RUN_TEST(test_default_values);
    // Link struct after list of one. Verify links. Verify containers unmodified
    RUN_TEST(test_add_after_last_node);
    // Link struct in middle of list of three. Verify links.
    RUN_TEST(test_add_after_mid_node);
    // Link struct before list of one. Verify links. Verify containers unmodified
    RUN_TEST(test_add_before_first_node);
    // Link struct in middle of list of three. Verify links.
    RUN_TEST(test_add_before_mid_node);

    UNITY_END();
}
