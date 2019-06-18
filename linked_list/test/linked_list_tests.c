/*
 * Unit tests for the linked list.
 * See Unity's 'Getting Started' doc for information on the idiom used here
 *
 * Written by Max Hanson, June 2019.
 */

#include "../linked_list.h"
#include "unity.h"

static void *malloc_int(int i);
static int test_validity(LinkedList *list);


/*
 * Create a small linked list of ints and verify the keys.
 */
void test_int_list_keys()
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
 * Test the add_at operation.
 *
 * Test that the operation increments the size, adds the correct node, and that
 * the list is valid afterwards.
 */
void test_add_at()
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

// TODO put constructor/destructor in setup(), tearDown()?
int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_int_list_keys);
    RUN_TEST(test_empty_list_validity);
    RUN_TEST(test_add);
    RUN_TEST(test_add_at);
    RUN_TEST(test_remove_at);
    RUN_TEST(test_remove_key);
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
