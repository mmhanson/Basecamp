/*
 * A basic doubly linked list.
 *
 * === How To Use ===
 * This header defines a struct representing a linked list node (ListNode). All
 * linked list operations operate on these nodes. Instances of a struct can be
 * linked together by embedding a ListNode in the struct and managing the
 * instances with the operations below. The @container pointer in each node is
 * meant to point to the struct containing the node. This way it is easy to go
 * from a node to its containing struct. The diagram below should how three
 * structs (A, B, C) could be linked together.
 *
 *     A           B           C      
 * +-------+   +-------+   +-------+
 * |   ~   |<+ |   ~   |<+ |   ~   |<+
 * |+-----+| | |+-----+| | |+-----+| |
 * ||    -++-+ ||    -++-+ ||    -++-+
 * |+-----+|   |+-----+|   |+-----+|
 * ||    -++-->||    -++-->||  \  ||
 * |+-----+|   |+-----+|   |+-----+|
 * ||  \  ||<--++-    ||<--++-    ||
 * |+-----+|   |+-----+|   |+-----+|
 * +-------+   +-------+   +-------+
 *
 * Written by Max Hanson, June 2019.
 * Licensed under MIT. See LICENSE.txt for more details.
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H


typedef struct ListNodeTag ListNode;
struct ListNodeTag 
{
    void *container;
    ListNode *next;
    ListNode *prev;
};

/*
 * Create a list of one node.
 *
 * @node: The node to create a list. @node.next, @node.prev will be null.
 * @container: The struct containing the node. See usage above.
 */
static void list_init(ListNode *node, void *container)
{
    node->container = container;
    node->next = 0;
    node->prev = 0;
}

/*
 * Add @new_node after @node in the list.
 * O(1) complexity. @new_node doesnt need to be initialized, its members will be
 * overwritten anyway.
 *
 * @node: Add @new_node after this node in the list.
 * @new_node: Add this node after @node in the list.
 * @container: The struct which @new_node is contained in. See usage at top.
 */
static void list_add_after(ListNode *node, ListNode *new_node,
                                   void *new_node_container)
{
    new_node->container = new_node_container;

    if (node->next == 0)
    {
        /* @node is tail. */
        node->next = new_node;
        new_node->prev = node;
        new_node->next = 0;
    }
    else
    {
        /* @node is intermediate node. */
        new_node->next = node->next;
        new_node->prev = node;
        node->next->prev = new_node;
        node->next = new_node;
    }
}

/*
 * Add @new_node before @node in the list.
 * O(1) complexity. @new_node doesnt need to be initialized, its members will be
 * overwritten anyway.
 *
 * @node: Add @new_node before this node in the list.
 * @new_node: Add this node before @node in the list.
 * @container: The struct which @new_node is contained in. See usage at top.
 */
static void list_add_before(ListNode *node, ListNode *new_node,
                                    void *new_node_container)
{
    new_node->container = new_node_container;

    if (node->prev == 0)
    {
        /* @node is head. */
        node->prev = new_node;
        new_node->next = node;
        new_node->prev = 0;
    }
    else
    {
        /* @node is intermediate node. */
        new_node->next = node;
        new_node->prev = node->prev;
        node->prev->next = new_node;
        node->prev = new_node;
    }
}

/*
 * Remove (unlink) @node from the list.
 * O(1) complexity. Caution! @node will not be deallocated.
 *
 * @node: The node to be removed from the list.
 */
static void list_remove(ListNode *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = 0;
    node->prev = 0;
}

/*
 * Replace @old_node with @new_node.
 * O(1) complexity. Caution! @old_node is not deallocated.
 *
 * @old_node: The node to be replaced by @new_node.
 * @new_node: The node to replace @old_node with.
 */
static void list_replace(ListNode *old_node, ListNode *new_node)
{
    /* Link in new node. */
    new_node->next = old_node->next;
    new_node->prev = old_node->prev;

    /* Unlink old node. */
    old_node->prev->next = new_node;
    old_node->next->prev = new_node;
    old_node->next = 0;
    old_node->prev = 0;
}

/*
 * Swap @node_a and @node_b in the list.
 * O(1) complexity.
 *
 * @node_a: Will take @node_b's previous position in the list.
 * @node_b: Will take @node_a's previous position in the list.
 */
static void list_swap(ListNode *node_a, ListNode *node_b)
{
    ListNode *tmp_prev;
    ListNode *tmp_next;

    /* Put @node_b in @node_a's spot. */
    tmp_next = node_b->next; /* to put a in b's spot */
    tmp_prev = node_b->prev;
    node_b->prev = node_a->prev;
    node_b->next = node_a->next;
    node_b->next->prev = node_b;
    node_b->prev->next = node_b;

    /* Put @node_a in @node_b's spot. */
    node_a->next = tmp_next;
    node_a->prev = tmp_prev;
    node_a->next->prev = node_a;
    node_a->prev->next = node_a;
}

/*
 * Iterate through a linked list starting at @start_node.
 * Iteration starts at (and including) @start_node and goes through the list
 * including the tail.
 * 
 * @start_node: Pointer to node to start iteration at.
 * @cursor: Points to where iteration currently is.
 */
#define LIST_FOR_EACH_AFTER(start_node, cursor) \
    for (cursor = start_node; cursor != 0; cursor = cursor->next)

/*
 * Iterate over a linked list between two nodes.
 * Iteration starts at (and including) @start_node and goes up to (not
 * including) @end_node.
 *
 * @start_node: Node to start iteration at.
 * @end_node: Node to end iteration before.
 * @cursor: Points to where iteration currently is.
 */
#define LIST_FOR_EACH_BETWEEN(start_node, end_node, cursor) \
    for (cursor = start_node; cursor != end_node; cursor = cursor->next)


#endif
