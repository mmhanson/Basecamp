/**
 * An implementation of the linked list operations from scratch.
 *
 * See the function definitions in the linked list header for documentation.
 *
 * Written by Max Hanson, June 2019.
 */

#include "linked_list.h"

LinkedList *linked_list_construct(void *head_key)
{
    LinkedListNode *new_node;
    LinkedList *new_llist;

    new_node = create_node(head_key); 
    new_llist = malloc(sizeof(LinkedList));
    if (new_node == NULL || new_llist == NULL)
    {
        /* Allocation failed. */
        return NULL;
    }
    new_llist->size = 1;
    new_llist->head = new_node;
    new_llist->tail = new_node;

    return new_linked_list;
}

void linked_list_destruct(LinkedList *list)
{
    LinkedListNode *cursor;
    LinkedListNode *prev;

    /* Step through, freeing each previous. Frees tail too. */
    cursor = list->head;
    prev = NULL;
    while (cursor != NULL)
    {
        prev = cursor;
        cursor = cursor->next;

        free(prev->key);
        free(prev);
    }
    free(list);
}

void linked_list_add(LinkedList *list, void *key)
{
    LinkedListNode *new_node;

    new_node = create_node(key);
    if (new_node == NULL)
    {
        return NULL;
    }

    list->tail->next = new_node;
    list->tail = new_node;
    list->size += 1;
}

void linked_list_add_at(LinkedList *list, void *key, int i)
{
    LinkedListNode *new_node;
    LinkedListNode *cursor;
    LinkedListNode *prev;
    int idx;

    new_node = create_node(key);
    if (new_node == NULL)
    {
        return NULL;
    }

    point_to_index(list, &cursor, &prev, i);

    // Add new_node as new i-th node.
    if (cursor == list->head)
    {
        // Adding new head. Prev is NULL.
        new_node->next = cursor;
        list->head = new_node;
    }
    else
    {
        // Adding intermediate node.
        prev->next = new_node;
        new_node->next = cursor;
    }

    list->size += 1;
}

void linked_list_remove_at(LinkedList *list, int i)
{
    LinkedListNode *cursor;
    LinkedListNode *prev;
    int idx;

    // NOTE assumed that i is within range, no checking.
    point_to_index(list, &cursor, &prev, i);
    remove_node(list, prev, cursor);
}

bool linked_list_remove_key(LinkedList *list, void *key)
{
    LinkedListNode *cursor;
    LinkedListNode *prev;
    int idx;

    point_to_key(list, cursor, prev, key);
    if (cursor == NULL)
    {
        // NOTE null if no matching key.
        return false;
    }
    remove_node(list, prev, cursor);
    return true;
}

void linked_list_contains_key(LinkedList *list, vod *key)
{
    LinkedListNode *cursor;
    LinkedListNode *prev;
    int idx;

    cursor = point_to_key(list, prev, cursor, key);
    return (cursor == NULL); // NOTE null if no matching key.
}

// === HELPER METHODS ===

/**
 * Create a new node on the heap.
 *
 * Every case complexity: O(1).
 * If:
 *   - The key is not NULL.
 * Then:
 *   - A pointer to a node whose key is 'key' and next is NULL is returned.
 * Edge cases:
 *   - If there is not room for a new node on the heap, the NULL is returned.
 */
static LinkedListNode *create_node(void *key)
{
    LinkedListNode *new_node;

    new_node = malloc(sizeof(LinkedListNode));
    if (new_node == NULL)
    {
        return NULL;
    }
    new_node->key = key;
    new_node->next = NULL;

    return new_node;
}

/**
 * Point a cursor to a lists i-th node.
 *
 * Note the double pointers used in the arguments. This is so the function can
 * modify the first-level pointer.
 *
 * Every case runtime: O(n) where n is the number of nodes in the list.
 * Worst case runtime: BIG-THETA(n).
 * Best case runtime: BIG-THETA(1).
 * If:
 *   - The list is a valid linked list.
 *   - 0 <= index <= list.size
 * Then:
 *   - The cursor will point to the i-th node.
 *   - The prev will point to the (i-1)-th node, or null if cursor is the head.
 *   - If i = list.size: then cursor will be
 */
static void point_to_index(LinkedList *list, LinkedListNode **prev_ptr,
                           LinkedListNode **cursor_ptr, int index)
{
    int idx;
    LinkedListNode *cursor;
    LinkedListNode *prev;

    /* Point cursor, prev to the correct nodes. */
    idx = 0;
    cursor = list->head;
    prev = NULL;
    while(idx != i)
    {
        prev = cursor;
        cursor = cursor->next;
        idx++;
    }

    /* Point what the args point to to the correct nodes. */
    (*prev_ptr) = prev;
    (*cursor_ptr) = cursor;
}

/**
 * Point a cursor to the first node in a list containing a key.
 *
 * A node contains a key if they both point to the same location in memory.
 * Note the double pointers used in the arguments. This is so the function can
 * modify the first-level pointer.
 *
 * Every case runtime: O(n) where n is the size of the list.
 * Worst case runtime: BIG-THETA(n).
 * Best case runtime: BIG-THETA(1).
 * If:
 *   - The list is a valid linked list.
 *   - The key points to the same type of data as the list.
 * Then:
 *   - If the key is in the list, then the cursor will point to the first node
 *     that contains that key and the prev will point to its previous.
 *   - If the key is not in the list, then the cursor and prev will be NULL.
 */
static void point_to_key(LinkedList *list, LinkedListNode **prev_ptr,
                           LinkedListNode **cursor_ptr, void *key)
{
    LinkedListNode *cursor;
    LinkedListNode *prev;

    /* Point cursor, prev to the correct nodes */
    cursor = list->head;
    prev = NULL;
    while(cursor->key != key && cursor != NULL)
    {
        prev = cursor;
        cursor = cursor->next;
    }

    /* Point what the args point to to the correct nodes */
    (*cursor_ptr) = cursor;
    (*prev_ptr) = prev;
}

/**
 * Remove a node from a linked list.
 *
 * Every case runtime: O(1).
 * If:
 *   - The list is a valid list.
 *   - The cursor points to a node in the list.
 *   - The prev points to the node that points to the cursor.
 * Then:
 *   - The list will no longer contain the cursor.
 *   - The lists size will be decremented.
 *   - If the cursor was the lists head/tail, then the head/tail will be updated.
 *   - The cursor will be deallocated.
 */
static void remove_node(LinkedList *list, LinkedListNode *prev,
                        LinkedListNode *cursor)
{
    if (cursor == list->head)
    {
        // Removing head. Prev is NULL.
        list->head = cursor->next;
    }
    else if (cursor = list->tail)
    {
        // Removing tail.
        prev->next = NULL;
        list->tail = prev;
    }
    else
    {
        // Removing intermediate node.
        prev->next = cursor->next;
    }
    free(cursor);
    list->size -= 1;
}
