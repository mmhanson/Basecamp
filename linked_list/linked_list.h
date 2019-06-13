/**
 * A basic, generic forward-linked-list data structure and relevant operations
 * implemented from scratch in pure, old fashioned C.
 *
 * All functions in this header are prepended with 'linked_list_' to create a
 * psuedo-namespace.
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/**
 * A generic forward linked-list data structure.
 *
 * Note that by 'generic' I mean the key is a void pointer that can point to any data
 * type. It is the client's responsibility to track what data type the linked list's
 * keys are and safely cast them to/from void pointers.
 *
 * A LinkedList is valid if, and only if:
 *   - The key of all nodes points to the same type of data.
 *   - The next node of all nodes either points to a node or is NULL (tail).
 *   - There are no loops in the list.
 *   - The head node leads to the tail node through an arbitrary number of 'next's.
 *   - The tail nodes next node is NULL.
 */
typedef struct linked_list_node_tag
{
    void *key;
    LinkedListNode *next;
} LinkedListNode;

typedef struct linked_list_tag
{
    int size;
    LinkedListNode *head;
    LinkedListNode *tail;
} LinkedList;


/**
 * Create a new linked list with one element.
 *
 * Every case complexity: BIG-THETA(1).
 * If:
 *   - 'head_key' is not NULL.
 * Then:
 *   - A valid LinkedList with a head node such that:
 *     - Its 'key' is 'head_key'.
 *     - Its 'next' is NULL.
 */
LinkedList *linked_list_new(void *head_key);

/**
 * Add an element to the back of a linked list.
 *
 * Every case complexity: BIG-THETA(1).
 * If:
 *   - 'list' is a valid LinkedList.
 *   - 'key' points to the same data type as all nodes of 'list'.
 * Then:
 *   -  The tail of 'list' will now point to a new node whose key is 'key' and whose
 *      'next' is NULL.
 */
void linked_list_add(LinkedList *list, void *key);

/**
 * Add an element to the i-th index of a linked list.
 *
 * Every case complexity: O(n) where n is the number of nodes in 'list'.
 * Worst case complexity: BIG-THETA(n) where n is the number of nodes in 'list'.
 * Best case complexity: BIG-THETA(1).
 * If:
 *   - 'list' is a valid LinkedList.
 *   - 'key' points to the same data type as all nodes of 'list'.
 *   - 0 <= 'i' < list.size
 * Then:
 *   - The node at the i-th index of 'list' will be a new node whose key is 'key' and
 *     whose 'next' points to the previous node at the i-th index.
 *   - The node at the (i-1)-th index of 'list' (if any) will point to this new node.
 */
void linked_list_add_at(LinkedList *list, void *key, int i);

/**
 * Remove the i-th node of a linked list.
 *
 * Every case complexity: O(n) where n is the number of elements in 'list'.
 * Worst case complexity: BIG-THETA(n) where n is the number of nodes in 'list'.
 * Best case complexity: BIG-THETA(1).
 * If:
 *   - 'list' is a valid LinkedList.
 *   - 0 <= i < 'list.size'.
 * Then:
 *   - The node at the i-th index of 'list' will be removed.
 *   - The (i-1)-th node will point to the (previously) (i+1)-th node of 'list'.
 */
void linked_list_remove_at(LinkedList *list, int i);

/**
 * Remove the node of a linked list containing a certain key.
 *
 * Every case complexity: O(n) where n is the number of elements in 'list'.
 * Worst case complexity: BIG-THETA(n) where n is the number of nodes in 'list'.
 * Best case complexity: BIG-THETA(1).
 * If:
 *   - 'list' is a valid LinkedList.
 *   - 'key' is not NULL.
 * Then:
 *   - The first node, if any, whose key equals 'key' (comparing addresses) will be
 *     removed from 'list'.
 *   - If no nodes in 'list' contain 'key', then 'list' is unchanged.
 */
void linked_list_remove_key(LinkedList *list, void *key);

#endif
