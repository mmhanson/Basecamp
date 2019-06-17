/*
 * A basic, generic forward-linked-list data structure and relevant operations
 * implemented from scratch in pure, old fashioned C.
 *
 * All functions in this header are prepended with 'linked_list_' to create a
 * psuedo-namespace. The linked list supports operations to: create a new linked
 * list, add an element (at the end or at an index), remove an element (from the
 * end or by key), test if a key is in the list, and determine the size of the
 * list.
 *
 * Written by Max Hanson, June 2019.
 */


#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h> /* For malloc/NULL */


/*
 * A generic forward-linked-list data structure.
 *
 * Note that by 'generic' I mean the key is a void pointer that can point to any
 * data type. It is the client's responsibility to track what data type the
 * linked list's keys are and safely cast them to/from void pointers.
 * Also note that it is necessary that each key in the list points to an object
 * on the heap that was allocated with 'malloc/calloc/realloc'. This is because
 * each key is freed in the linked_list_destruct operation. Undefined behavior
 * will occur if this requirement is not met.
 *
 * A LinkedList is valid if, and only if:
 *   - The key of all nodes points to the same type of data.
 *   - The key of all nodes points to an object on the heap.
 *   - The next node of all nodes either points to a node or is NULL (tail).
 *   - There are no loops in the list.
 *   - The head node leads to the tail node.
 *   - The tail nodes next node is NULL.
 */
typedef struct linked_list_node_tag LinkedListNode;
struct linked_list_node_tag
{
    void *key;
    LinkedListNode *next;
};
typedef struct linked_list_tag
{
    int size;
    LinkedListNode *head;
    LinkedListNode *tail;
} LinkedList;

/*
 * Create a new linked list from a key.
 *
 * Every case complexity: BIG-THETA(1).
 * If:
 *   - The head_key is not NULL.
 * Then:
 *   - A valid LinkedList is returned such that:
 *     - Its head nodes key is the head_key.
 *     - Its head node is its tail node.
 *     - Its size is 1.
 * Edge cases:
 *   - If there is not enough space on the heap for a new LinkedList and
 *     LinkedListNode, then NULL is returned.
 */
LinkedList *linked_list_construct(void *head_key);

/*
 * Deallocate a linked list.
 *
 * 
 * Every case runtime: BIG-THETA(n) where n is the size of the list.
 * If:
 *   - The list is a valid linked list.
 * Then:
 *   - All keys in the list will be freed.
 *   - All nodes in the list will be freed.
 *   - The linked list itself will be freed.
 *   - The list pointer will be NULL.
 */
void linked_list_destruct(LinkedList *list);

/*
 * Append a key to a linked list.
 *
 * Every case complexity: BIG-THETA(1).
 * If:
 *   - The list is a valid LinkedList.
 *   - The key points to the same data type as all nodes of the list.
 * Then:
 *   - A new node is created whose key is The key.
 *   - The lists previous tail will point to this new node.
 *   - The lists tail will point to this new node.
 *   - The lists size will be incremented.
 *   - A positive value is returned.
 * Edge cases:
 *   - If there is not enough space on the heap for a new LinkedListNode, then
 *     zero is returned.
 */
int linked_list_add(LinkedList *list, void *key);

/*
 * Add an element to the i-th index of a linked list.
 *
 * Every case complexity: O(n) where n is the number of nodes in the list.
 * Worst case complexity: BIG-THETA(n).
 * Best case complexity: BIG-THETA(1).
 * If:
 *   - The list is a valid LinkedList.
 *   - The key points to the same data type as all nodes of the list.
 *   - 0 <= 'i' <= list.size.
 * Then:
 *   - A new node is created whose key is The key.
 *   - This new node will be the i-th node of the list.
 *   - If a new head is added, then the lists head will be updated.
 *   - The lists size will be incremented.
 *   - A positive value is returned.
 * Edge cases:
 *   - If there is not enough space on the heap for a new LinkedListNode, then
 *     zero is returned.
 */
int linked_list_add_at(LinkedList *list, void *key, int i);

/*
 * Remove the i-th node of a linked list.
 *
 * Every case complexity: O(n) where n is the number of elements in the list.
 * Worst case complexity: BIG-THETA(n).
 * Best case complexity: BIG-THETA(1).
 * If:
 *   - The list is a valid LinkedList.
 *   - 0 <= i < 'list.size'.
 * Then:
 *   - The node previously at the lists i-th index will be removed.
 *   - If the head/tail is removed, then the new head/tail will be updated.
 *   - The lists size will be decremented.
 */
void linked_list_remove_at(LinkedList *list, int i);

/*
 * Remove the first node containing a certain key from a linked list.
 *
 * Every case complexity: O(n) where n is the number of elements in the list.
 * Worst case complexity: BIG-THETA(n).
 * Best case complexity: BIG-THETA(1).
 * If:
 *   - The list is a valid LinkedList.
 *   - The key is not NULL.
 * Then:
 *   - If the list contains the key, the first node with the matching key
 *     (comparing addresses) is removed and a positive value is returned.
 *   - If the list doesnt contain the key, then the list is unchanged and zero
 *     is returned.
 */
int linked_list_remove_key(LinkedList *list, void *key);

/*
 * Determine if a linked list contains a key.
 *
 * Every case complexity: O(n) where n is the nubmer of elements in the list.
 * Worst case complexity: BIG-THETA(n).
 * Best case complexity: BIG-THETA(1).
 * If:
 *   - The list is a valid LinkedList.
 *   - The key is not NULL.
 * Then:
 *   - If the list contains the key, then a positive value is returned.
 *   - If the list doesnt contain the key then zero is returned.
 */
int linked_list_contains_key(LinkedList *list, void *key);

/*
 * Get the key of the i-th node of a linked list.
 *
 * Every case complexity: O(n) where n is the number of elements in the list.
 * Worst case complexity: BIG-THETA(n).
 * Best case complexity: BIG-THETA(1).
 * If:
 *   - The list is a valid LinkedList.
 *   - 0 <= i < list.size
 * Then:
 *   - The key of the i-th node of the list is returned.
 */
void *linked_list_get_key(LinkedList *list, int i);


#endif
