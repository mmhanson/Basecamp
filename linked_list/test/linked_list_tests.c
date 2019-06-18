/*
 * Unit tests for the linked list.
 * See Unity's 'Getting Started' doc for information on the idiom used here
 *
 * Written by Max Hanson, June 2019.
 */

#include <stdlib.h> /* for random number gen in stress test */
#include "../linked_list.h"
#include "unity.h"

static void *malloc_int(int i);
static int test_validity(LinkedList *list);


/*
 * Test the get_key operation.
 *
 * Make a small linked list and test that the get_key op returns the correct
 * keys. Also tests basic correctness construction/adding of keys.
 */
void test_get_key()
{
    const int list_size = 10; /* size of list to test */
    int idx;
    int *key;
    LinkedList *list;

    /* Create list with nodes [0..list_size) */
    list = linked_list_construct(malloc_int(0)); 
    for (idx = 1; idx < list_size; idx++)
    {
        linked_list_add(list, malloc_int(idx));
    }

    /* Test that each list node is [0..9] */
    for (idx = 0; idx < list_size; idx++)
    {
        key = linked_list_get_key(list, idx);
        TEST_ASSERT_EQUAL_INT(*key, idx);
    }
    TEST_ASSERT(test_validity(list) > 0);

    linked_list_destruct(list);
}

/*
 * Test that a linked list of one node is valid.
 */
void test_empty_list_validity()
{
    LinkedList *list;

    list = linked_list_construct(malloc_int(0)); 
    TEST_ASSERT(test_validity(list) > 0);
    linked_list_destruct(list);
}

/**
 * Test that the add operation.
 *
 * Test that the operation incremenets the size, adds the correct node and that
 * the list is valid afterwards
 */
void test_add()
{
    const int list_size = 10;
    int prev_size;
    int idx;
    int *key;
    LinkedList *list;

    /* Create list with nodes [0..list_size) */
    list = linked_list_construct(malloc_int(0)); 
    for (idx = 1; idx < list_size; idx++)
    {
        linked_list_add(list, malloc_int(idx));
    }

    /* Add '42' to the end */
    prev_size = list->size;
    linked_list_add(list, malloc_int(42));
    
    /* Verify the operation */
    TEST_ASSERT(list->size == (prev_size + 1));
    key = linked_list_get_key(list, list->size - 2);
    TEST_ASSERT_EQUAL_INT(9, *key); /* verify key before tail */
    key = linked_list_get_key(list, list->size - 1);
    TEST_ASSERT_EQUAL_INT(42, *key); /* verify tail key */
    TEST_ASSERT(test_validity(list) > 0);

    linked_list_destruct(list);
}

/*
 * Add a node into the middle of the list with add_at.
 *
 * Test that the operation increments the size, adds the correct node, and that
 * the list is valid afterwards.
 */
void test_add_at_mid()
{
    const int list_size = 10;
    int prev_size;
    int idx;
    int add_idx;
    int *key;
    LinkedList *list;

    /* Create list with nodes [0..list_size) */
    list = linked_list_construct(malloc_int(0)); 
    for (idx = 1; idx < list_size; idx++)
    {
        linked_list_add(list, malloc_int(idx));
    }

    /* Add '42' at the 5th index */
    add_idx = 5;
    prev_size = list->size;
    linked_list_add_at(list, malloc_int(42), add_idx);

    /* Verify the operation */
    TEST_ASSERT_EQUAL_INT(prev_size + 1, list->size);
    key = linked_list_get_key(list, add_idx - 1);
    TEST_ASSERT_EQUAL_INT(4, *key); /* verify key before new node */
    key = linked_list_get_key(list, add_idx);
    TEST_ASSERT_EQUAL_INT(42, *key); /* verify new node */
    key = linked_list_get_key(list, add_idx + 1);
    TEST_ASSERT_EQUAL_INT(5, *key); /* verify key after new node */
    TEST_ASSERT(test_validity(list) > 0);

    linked_list_destruct(list);
}

/*
 * Add a node into the end of the list with add_at.
 *
 * Test that the operation increments the size, adds the correct node, and that
 * the list is valid afterwards.
 */
void test_add_at_end()
{
    const int list_size = 10;
    int prev_size;
    int idx;
    int add_idx;
    int *key;
    LinkedList *list;

    /* Create list with nodes [0..list_size) */
    list = linked_list_construct(malloc_int(0)); 
    for (idx = 1; idx < list_size; idx++)
    {
        linked_list_add(list, malloc_int(idx));
    }

    /* Add '42' at the 10th index */
    add_idx = 10;
    prev_size = list->size;
    linked_list_add_at(list, malloc_int(42), add_idx);

    /* Verify the operation */
    TEST_ASSERT_EQUAL_INT(prev_size + 1, list->size);
    key = linked_list_get_key(list, add_idx - 1);
    TEST_ASSERT_EQUAL_INT(9, *key); /* verify key before new node */
    key = linked_list_get_key(list, add_idx);
    TEST_ASSERT_EQUAL_INT(42, *key); /* verify new node */
    TEST_ASSERT(test_validity(list) > 0);

    linked_list_destruct(list);
}

/*
 * Test the remove_at operation.
 *
 * Test that the remove operation decrement the size, remove the correct nodes,
 * and that the list is valid afterwards.
 */
void test_remove_at()
{
    const int list_size = 10;
    int prev_size;
    int rem_idx;
    int idx;
    int *key;
    LinkedList *list;

    /* Create list with nodes [0..list_size) */
    list = linked_list_construct(malloc_int(0)); 
    for (idx = 1; idx < list_size; idx++)
    {
        linked_list_add(list, malloc_int(idx));
    }

    /* Remove the 5th node */
    rem_idx = 5;
    prev_size = list->size;
    linked_list_remove_at(list, rem_idx);

    /* Verify the list */
    TEST_ASSERT_EQUAL_INT(prev_size - 1, list->size);
    /* Verify that the correct node was removed (list goes ... 3, 4, 6 ...) */
    key = linked_list_get_key(list, rem_idx - 1);
    TEST_ASSERT_EQUAL_INT(4, *key);
    key = linked_list_get_key(list, rem_idx);
    TEST_ASSERT_EQUAL_INT(6, *key);
    TEST_ASSERT(test_validity(list) > 0);

    linked_list_destruct(list);
}

/*
 * Test the remove_key operation.
 *
 * Test that it decrements the size, removes the correct nodes, and that the
 * list is valid afterwards.
 */
void test_remove_key()
{
    const int list_size = 10;
    int prev_size;
    int idx;
    int rem_idx;
    int *rem_key;
    int *key;
    LinkedList *list;

    /* Create list with nodes [0..list_size) */
    list = linked_list_construct(malloc_int(0)); 
    rem_idx = 5; /* remove the 5th node */
    for (idx = 1; idx < list_size; idx++)
    {
        key = malloc_int(idx);
        linked_list_add(list, key);
        
        /* Save key to remove later */
        if (idx == rem_idx)
        {
            rem_key = key;
        }
    }

    /* Remove '5' */
    prev_size = list->size;
    linked_list_remove_key(list, rem_key);

    /* Verify the list */
    TEST_ASSERT_EQUAL_INT(prev_size - 1, list->size);
    /* Verify that the correct node was removed (list goes ... 3, 4, 6 ...) */
    key = linked_list_get_key(list, rem_idx - 1);
    TEST_ASSERT_EQUAL_INT(4, *key);
    key = linked_list_get_key(list, rem_idx);
    TEST_ASSERT_EQUAL_INT(6, *key);
    TEST_ASSERT(test_validity(list) > 0);

    linked_list_destruct(list);
}

/*
 * Test the remove_key operation when the key is not in the list.
 *
 * Verify that the list is unchanged and that zero is returned.
 */
void test_remove_key_not_contained()
{
    const int list_size = 10;
    int prev_size;
    int idx;
    int retval;
    int rem_int;
    int *rem_key;
    int *key;
    LinkedList *list;

    /* Create list with nodes [0..list_size) */
    list = linked_list_construct(malloc_int(0)); 
    for (idx = 1; idx < list_size; idx++)
    {
        key = malloc_int(idx);
        linked_list_add(list, key);
    }

    /* Attempt removing the non-contained rem_key */
    rem_int = 1234;
    rem_key = &rem_int; /* attempt removing a random int pointer */
    prev_size = list->size;
    retval = linked_list_remove_key(list, rem_key);

    /* Verify the list is unchanged */
    TEST_ASSERT_EQUAL_INT(0, retval);
    TEST_ASSERT_EQUAL_INT(prev_size, list->size);
    for (idx = 0; idx < list_size; idx++)
    {
        key = linked_list_get_key(list, idx);
        TEST_ASSERT_EQUAL_INT(idx, *key); /* verify all keys unchanged */
    }
    TEST_ASSERT(test_validity(list) > 0);

    linked_list_destruct(list);
}

/*
 * Test the contains_key operation when the key is contained in the list.
 */
void test_contains_key()
{
    const int list_size = 10;
    int idx;
    int retval;
    int cont_idx;
    int *cont_key;
    int *key;
    LinkedList *list;
    
    /* Create list with nodes [0..list_size) */
    cont_idx = 5;
    list = linked_list_construct(malloc_int(0)); 
    for (idx = 1; idx < list_size; idx++)
    {
        key = malloc_int(idx);
        linked_list_add(list, key);

        if (idx == cont_idx)
        {
            cont_key = key;
        }
    }

    retval = linked_list_contains_key(list, cont_key);
    TEST_ASSERT(retval > 0);
    TEST_ASSERT(test_validity(list) > 0);

    linked_list_destruct(list);
}

/*
 * Test the contains_key operation when the key is not contained in the list.
 */
void test_contains_key_not_contained()
{
    const int list_size = 10;
    int idx;
    int retval;
    int cont_key;
    int *cont_key_ptr;
    int *key;
    LinkedList *list;
    
    /* Create list with nodes [0..list_size) */
    list = linked_list_construct(malloc_int(0)); 
    for (idx = 1; idx < list_size; idx++)
    {
        key = malloc_int(idx);
        linked_list_add(list, key);
    }

    cont_key = 1234;
    cont_key_ptr = &cont_key; /* test contains a random int pointer */
    retval = linked_list_contains_key(list, cont_key_ptr);
    TEST_ASSERT(retval == 0);
    TEST_ASSERT(test_validity(list) > 0);

    linked_list_destruct(list);
}

/*
 * Stress-test the linked list.
 *
 * Adds many elements to the list, then verifies the validity of the list. Then
 * removes many elements from the list and verifies the validity. Then adds many
 * more elements back into the list and verifies the validity. Then combo adds/
 * removes many elements and verifies the validity. Then removes all elements
 * (except one) and verifies the validity.
 */
void test_stress()
{
    const int init_list_size = 500;
    const int rem_num = 200;
    const int add_num = 300;
    int idx;
    int rem_idx; /* index to remove or add at */
    int exp_size; /* expected sizes */
    int *key;
    LinkedList *list;
    
    /* Create list with nodes [0..list_size) */
    list = linked_list_construct(malloc_int(0)); 
    for (idx = 1; idx < init_list_size; idx++)
    {
        key = malloc_int(idx);
        linked_list_add(list, key);
    }

    /* Remove 'rem_num' elements from random indexes */
    for (idx = 0; idx < rem_num; idx++)
    {
        rem_idx = rand() % list->size; /* gen rand # in [0..list->size) */
        linked_list_remove_at(list, rem_idx);
    }
    exp_size = init_list_size - rem_num;
    TEST_ASSERT_EQUAL_INT(exp_size, list->size);
    TEST_ASSERT(test_validity(list) > 0);

    /* Add 'add_num' elements from random indexes */
    // TODO segfault
    for (idx = 0; idx < add_num; idx++)
    {
        rem_idx = rand() % list->size; /* gen rand # in [0..list->size) */
        key = malloc_int(rand() % 1000); /* key is rand # in [0..1000) */
        linked_list_add_at(list, key, rem_idx);
    }
    exp_size = exp_size + add_num;
    TEST_ASSERT_EQUAL_INT(exp_size, list->size);
    TEST_ASSERT(test_validity(list) > 0);

    /* Add 'add_num' elems and remove 'rem_num' elems from random indexes */
    for (idx = 0; idx < add_num; idx++)
    {
        rem_idx = rand() % list->size; /* gen rand # in [0..list->size) */
        key = malloc_int(rand() % 1000); /* key is rand # in [0..1000) */
        linked_list_add_at(list, key, rem_idx);
    }
    for (idx = 0; idx < rem_num; idx++)
    {
        rem_idx = rand() % list->size; /* gen rand # in [0..list->size) */
        linked_list_remove_at(list, rem_idx);
    }
    exp_size = exp_size + add_num - rem_num;
    TEST_ASSERT_EQUAL_INT(exp_size, list->size);
    TEST_ASSERT(test_validity(list) > 0);

    /* Remove all but the head */
    while (list->head != list->tail)
    {
        linked_list_remove_at(list, list->size - 1);
    }
    exp_size = 1;
    TEST_ASSERT_EQUAL_INT(exp_size, list->size);
    TEST_ASSERT(test_validity(list) > 0);

    linked_list_destruct(list);
}

// TODO put constructor/destructor in setup(), tearDown()?
int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_get_key);
    RUN_TEST(test_empty_list_validity);
    RUN_TEST(test_add);
    RUN_TEST(test_add_at_mid);
    RUN_TEST(test_add_at_end);
    RUN_TEST(test_remove_at);
    RUN_TEST(test_remove_key);
    RUN_TEST(test_remove_key_not_contained);
    RUN_TEST(test_contains_key);
    RUN_TEST(test_contains_key_not_contained);
    RUN_TEST(test_stress);
    UNITY_END();
}

static void *malloc_int(int i)
{
    int *int_ptr = malloc(sizeof(int));
    *int_ptr = i;
    return int_ptr;
}

/* 
 * Test that a linked list is valid. See header for details on validity.
 * 
 * Does not check if all keys are the same type.
 *
 * Every case complexity: BIG-THETA(n^2) where n is the nodes in the list.
 * If:
 *   - The list is a valid list.
 * Then:
 *   - A positive value is returned.
 * Edge cases:
 *   - If the list is invalid, then zero is returned.
 */
static int test_validity(LinkedList *list)
{
    int idx;
    LinkedListNode *cursor;
    int subidx; /* for indexing nested loop */
    LinkedListNode *subcursor;

    /* Check for loops. Verify each nodes next is not previously in the list. */
    /* Cursor will point to the tail when loop is done. */
    cursor = list->head;
    for (idx = 0; idx < (list->size - 1); idx++)
    {
        subcursor = list->head;
        for (subidx = 0; subidx < idx; subidx++)
        {
            if (subcursor == cursor->next)
            {
                return 0;
            }
        }
        cursor = cursor->next;

        /* Happens when list.size is too small */
        if (cursor == NULL)
        {
            return 0;
        }
    }

    /* Head does not lead to the tail */
    if (cursor != list->tail)
    {
        return 0;
    }

    /* Tails next must be null */
    if (list->tail->next != NULL)
    {
        return 0;
    }

    return 1;
}
