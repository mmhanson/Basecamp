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
    ContainerStruct *head;

    head = malloc(sizeof(ContainerStruct));
    head->dummy_mem = 0;
    list_init(&head->node, head);

    TEST_ASSERT_EQUAL(head, head->node.container);
    TEST_ASSERT_EQUAL(0, head->node.next);
    TEST_ASSERT_EQUAL(0, head->node.prev);

    free(head);
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
    ContainerStruct *head, *midd, *tail, *to_add;

    head = malloc(sizeof(ContainerStruct));
    midd = malloc(sizeof(ContainerStruct));
    tail = malloc(sizeof(ContainerStruct));
    to_add = malloc(sizeof(ContainerStruct));
    list_init(&head->node, head);
    list_add_after(&head->node, &midd->node, midd);
    list_add_after(&midd->node, &tail->node, tail);

    list_add_after(&midd->node, &to_add->node, to_add);
    // List should be: head <-> midd <-> to_add <-> tail

    // Assert head points to correct nodes.
    TEST_ASSERT_EQUAL(0, head->node.prev);
    TEST_ASSERT_EQUAL(&midd->node, head->node.next);

    // Assert midd points to correct nodes.
    TEST_ASSERT_EQUAL(&head->node, midd->node.prev);
    TEST_ASSERT_EQUAL(&to_add->node, midd->node.next);

    // Assert to_add points to correct nodes.
    TEST_ASSERT_EQUAL(&midd->node, to_add->node.prev);
    TEST_ASSERT_EQUAL(&tail->node, to_add->node.next);

    // Assert tail points to correct nodes.
    TEST_ASSERT_EQUAL(&to_add->node, tail->node.prev);
    TEST_ASSERT_EQUAL(0, tail->node.next);

    free(head);
    free(midd);
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
    ContainerStruct *head, *midd, *tail, *to_add;

    head = malloc(sizeof(ContainerStruct));
    midd = malloc(sizeof(ContainerStruct));
    tail = malloc(sizeof(ContainerStruct));
    to_add = malloc(sizeof(ContainerStruct));
    list_init(&head->node, head);
    list_add_after(&head->node, &midd->node, midd);
    list_add_after(&midd->node, &tail->node, tail);

    list_add_before(&tail->node, &to_add->node, to_add);
    // List should be: head <-> midd <-> to_add <-> tail

    // Assert head points to correct nodes.
    TEST_ASSERT_EQUAL(0, head->node.prev);
    TEST_ASSERT_EQUAL(&midd->node, head->node.next);

    // Assert midd points to correct nodes.
    TEST_ASSERT_EQUAL(&head->node, midd->node.prev);
    TEST_ASSERT_EQUAL(&to_add->node, midd->node.next);

    // Assert to_add points to correct nodes.
    TEST_ASSERT_EQUAL(&midd->node, to_add->node.prev);
    TEST_ASSERT_EQUAL(&tail->node, to_add->node.next);

    // Assert tail points to correct nodes.
    TEST_ASSERT_EQUAL(&to_add->node, tail->node.prev);
    TEST_ASSERT_EQUAL(0, tail->node.next);

    free(head);
    free(midd);
    free(to_add);
    free(tail);
}

void test_remove_list_of_one()
{
    ContainerStruct *head;

    head = malloc(sizeof(ContainerStruct));
    head->dummy_mem = 1;
    list_init(&head->node, head);

    list_remove(&head->node);

    TEST_ASSERT_EQUAL(0, head->node.next); // test links same.
    TEST_ASSERT_EQUAL(0, head->node.prev);
    TEST_ASSERT_EQUAL(1, head->dummy_mem); // test container unmodified
}

void test_remove_head()
{
    ContainerStruct *head, *midd, *tail;

    head = malloc(sizeof(ContainerStruct));
    midd = malloc(sizeof(ContainerStruct));
    tail = malloc(sizeof(ContainerStruct));
    head->dummy_mem = 1;
    midd->dummy_mem = 2;
    tail->dummy_mem = 3;
    list_init(&head->node, head);
    list_add_after(&head->node, &midd->node, midd);
    list_add_after(&midd->node, &tail->node, tail);

    list_remove(&head->node);
    // List should go: midd <-> tail

    // Test links in chain.
    TEST_ASSERT_EQUAL(0, midd->node.prev);
    TEST_ASSERT_EQUAL(0, tail->node.next);
    TEST_ASSERT_EQUAL(&tail->node, midd->node.next);
    TEST_ASSERT_EQUAL(&midd->node, tail->node.prev);

    // Test head is now unlinked from chain.
    TEST_ASSERT_EQUAL(0, head->node.next);
    TEST_ASSERT_EQUAL(0, head->node.prev);

    // Test members unmodified.
    TEST_ASSERT_EQUAL(1, head->dummy_mem);
    TEST_ASSERT_EQUAL(2, midd->dummy_mem);
    TEST_ASSERT_EQUAL(3, tail->dummy_mem);
}

void test_remove_tail()
{
    ContainerStruct *head, *midd, *tail;

    head = malloc(sizeof(ContainerStruct));
    midd = malloc(sizeof(ContainerStruct));
    tail = malloc(sizeof(ContainerStruct));
    head->dummy_mem = 1;
    midd->dummy_mem = 2;
    tail->dummy_mem = 3;
    list_init(&head->node, head);
    list_add_after(&head->node, &midd->node, midd);
    list_add_after(&midd->node, &tail->node, tail);

    list_remove(&tail->node);
    // List should go: head <-> midd 

    // Test links in chain.
    TEST_ASSERT_EQUAL(0, head->node.prev);
    TEST_ASSERT_EQUAL(0, midd->node.next);
    TEST_ASSERT_EQUAL(&midd->node, head->node.next);
    TEST_ASSERT_EQUAL(&head->node, midd->node.prev);

    // Test tail is now unlinked from chain.
    TEST_ASSERT_EQUAL(0, tail->node.next);
    TEST_ASSERT_EQUAL(0, tail->node.prev);

    // Test members unmodified.
    TEST_ASSERT_EQUAL(1, head->dummy_mem);
    TEST_ASSERT_EQUAL(2, midd->dummy_mem);
    TEST_ASSERT_EQUAL(3, tail->dummy_mem);
}

void test_remove_mid_node()
{
    ContainerStruct *head, *midd, *tail;

    head = malloc(sizeof(ContainerStruct));
    midd = malloc(sizeof(ContainerStruct));
    tail = malloc(sizeof(ContainerStruct));
    head->dummy_mem = 1;
    midd->dummy_mem = 2;
    tail->dummy_mem = 3;
    list_init(&head->node, head);
    list_add_after(&head->node, &midd->node, midd);
    list_add_after(&midd->node, &tail->node, tail);

    list_remove(&midd->node);
    // List should go: head <-> tail

    // Test links in chain.
    TEST_ASSERT_EQUAL(0, head->node.prev);
    TEST_ASSERT_EQUAL(0, tail->node.next);
    TEST_ASSERT_EQUAL(&tail->node, head->node.next);
    TEST_ASSERT_EQUAL(&head->node, tail->node.prev);

    // Test midd is now unlinked from chain.
    TEST_ASSERT_EQUAL(0, midd->node.next);
    TEST_ASSERT_EQUAL(0, midd->node.prev);

    // Test members unmodified.
    TEST_ASSERT_EQUAL(1, head->dummy_mem);
    TEST_ASSERT_EQUAL(2, midd->dummy_mem);
    TEST_ASSERT_EQUAL(3, tail->dummy_mem);
}

void test_replace_head()
{
    ContainerStruct *head, *midd, *tail, *to_add;

    head = malloc(sizeof(ContainerStruct));
    midd = malloc(sizeof(ContainerStruct));
    tail = malloc(sizeof(ContainerStruct));
    to_add = malloc(sizeof(ContainerStruct));
    head->dummy_mem = 1;
    midd->dummy_mem = 2;
    tail->dummy_mem = 3;
    to_add->dummy_mem = 4;
    list_init(&head->node, head);
    list_add_after(&head->node, &midd->node, midd);
    list_add_after(&midd->node, &tail->node, tail);

    list_replace(&head->node, &to_add->node);
    // List should go: to_add <-> midd <-> tail

    // Test links in chain.
    TEST_ASSERT_EQUAL(0, to_add->node.prev);
    TEST_ASSERT_EQUAL(0, tail->node.next);
    TEST_ASSERT_EQUAL(&midd->node, to_add->node.next);
    TEST_ASSERT_EQUAL(&to_add->node, midd->node.next);
    TEST_ASSERT_EQUAL(&tail->node, midd->node.next);
    TEST_ASSERT_EQUAL(&midd->node, tail->node.prev);

    // Test head is now unlinked from chain.
    TEST_ASSERT_EQUAL(0, head->node.next);
    TEST_ASSERT_EQUAL(0, head->node.prev);

    // Test members unmodified.
    TEST_ASSERT_EQUAL(1, head->dummy_mem);
    TEST_ASSERT_EQUAL(2, midd->dummy_mem);
    TEST_ASSERT_EQUAL(3, tail->dummy_mem);
    TEST_ASSERT_EQUAL(4, to_add->dummy_mem);
}

void test_replace_tail()
{
    ContainerStruct *head, *midd, *tail, *to_add;

    head = malloc(sizeof(ContainerStruct));
    midd = malloc(sizeof(ContainerStruct));
    tail = malloc(sizeof(ContainerStruct));
    to_add = malloc(sizeof(ContainerStruct));
    head->dummy_mem = 1;
    midd->dummy_mem = 2;
    tail->dummy_mem = 3;
    to_add->dummy_mem = 4;
    list_init(&head->node, head);
    list_add_after(&head->node, &midd->node, midd);
    list_add_after(&midd->node, &tail->node, tail);

    list_replace(&tail->node, &to_add->node);
    // List should go: head <-> midd <-> to_add 

    // Test links in chain.
    TEST_ASSERT_EQUAL(0, to_add->node.prev);
    TEST_ASSERT_EQUAL(0, tail->node.next);
    TEST_ASSERT_EQUAL(&midd->node, to_add->node.next);
    TEST_ASSERT_EQUAL(&to_add->node, midd->node.next);
    TEST_ASSERT_EQUAL(&tail->node, midd->node.next);
    TEST_ASSERT_EQUAL(&midd->node, tail->node.prev);

    // Test head is now unlinked from chain.
    TEST_ASSERT_EQUAL(0, head->node.next);
    TEST_ASSERT_EQUAL(0, head->node.prev);

    // Test members unmodified.
    TEST_ASSERT_EQUAL(1, head->dummy_mem);
    TEST_ASSERT_EQUAL(2, midd->dummy_mem);
    TEST_ASSERT_EQUAL(3, tail->dummy_mem);
    TEST_ASSERT_EQUAL(4, to_add->dummy_mem);
}

void test_replace_mid_node()
{
    
}

int main()
{
    UNITY_BEGIN();

    // Verify default values after initializing a list of one.
    RUN_TEST(test_default_values);

    // Link after list of one. Verify links. Verify containers unmodified.
    RUN_TEST(test_add_after_last_node);
    // Link struct in middle of list of three. Verify links.
    RUN_TEST(test_add_after_mid_node);
    // Link before list of one. Verify links. Verify containers unmodified.
    RUN_TEST(test_add_before_first_node);
    // Link struct in middle of list of three. Verify links.
    RUN_TEST(test_add_before_mid_node);

    // Remove a node in a list of one. Verify nothing happened.
    RUN_TEST(test_remove_list_of_one);
    // Remove the head of a list. Verify links. Verify containers unmodified.
    RUN_TEST(test_remove_head);
    // Remove the tail of a list. Verify links. Verify containers unmodified.
    RUN_TEST(test_remove_tail);
    // Remove from middle of a list. Verify links. Verify containers unmodified.
    RUN_TEST(test_remove_mid_node);

    // Replace the head of a list. Verify links and containers unmodified.
    RUN_TEST(test_replace_head);
    // Replace the tail of a list. Verify links and containers unmodified.
    RUN_TEST(test_replace_tail);
    // Replace at the middle of a list. Verify links and containers unmodified.
    RUN_TEST(test_replace_mid_node);

    UNITY_END();
}
