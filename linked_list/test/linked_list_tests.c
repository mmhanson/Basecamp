/*
 * Unit tests for the linked list.
 * See Unity's 'Getting Started' doc for information on the idiom used here
 *
 * Written by Max Hanson, June 2019.
 */

#include "../linked_list.h"
#include "unity.h"

static void *malloc_int(int i);


/*
 * Create a small linked list and test that the keys were created properly.
 */
void test_llist_keys()
{
    const int list_size = 10; /* size of list to test */
    int idx;
    int *key;
    LinkedList *llist;

    /* Create llist with nodes [0..list_size) */
    llist = linked_list_construct(malloc_int(0)); 
    for (idx = 1; idx < list_size; idx++)
    {
        linked_list_add(llist, malloc_int(idx));
    }

    /* Test that each llist node is [0..9] */
    for (idx = 0; idx < list_size; idx++)
    {
        key = linked_list_get_key(llist, idx);
        TEST_ASSERT_EQUAL_INT(*key, idx);
    }

    linked_list_destruct(llist);
}

// TODO put constructor/destructor in setup(), tearDown()?
int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_llist_keys);
    UNITY_END();
}

static void *malloc_int(int i)
{
    int *int_ptr = malloc(sizeof(int));
    *int_ptr = i;
    return int_ptr;
}
