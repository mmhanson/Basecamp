/*
 * A basic doubly linked list.
 *
 * === How To Use ===
 *  - TODO add info about embedding LListNode into structs.
 *
 * Written by Max Hanson, June 2019.
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct _LListNodeTag LListNode;
struct _LListNodeTag 
{
    void *container;
    LListNode *next;
    LListNode *prev;
};

/*
 * Initialize a linked list node to its default values.
 */
#define LLIST_INIT(container, head_var_name) {container, 0, 0}

/*
 * Add @new_node after @node in the list.
 * Every case complexity: O(1).
 * No need to initialize @new_node.
 *
 * @node: Add @new_node after this node in the list.
 * @new_node: Add this node after @node in the list.
 * @container: The struct which @new_node is contained in. See usage at top.
 */
static inline void llist_add_after(LListNode *node, LListNode *new_node,
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
 * Every case complexity: O(1).
 * No need to initialize @new_node.
 *
 * @node: Add @new_node before this node in the list.
 * @new_node: Add this node before @node in the list.
 * @container: The struct which @new_node is contained in. See usage at top.
 */
static inline void llist_add_before(LListNode *node, LListNode *new_node,
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
 * Does not deallocate the node after removal. This must be done by the client.
 * Every case complexity: O(1).
 * TODO doc that @node will still point into the list afterwards. Diagram?
 *
 * @node: The node to be removed from the list.
 */
static inline void llist_remove(LListNode *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

/*
 * Replace @old_node with @new_node.
 * Does not deallocate @old_node. This must be done by the client.
 * Every case complexity: O(1).
 * TODO doc that @old_node will point into the list afterwards. Diagram?
 */
static inline void llist_replace(LListNode *old_node, LListNode *new_node)
{
    /* Link in new node. */
    new_node->next = old_node->next;
    new_node->prev = old_node->prev;

    /* Unlink old node. */
    old_node->prev->next = new_node;
    old_node->next->prev = new_node;
}

/*
 * Swap @node_a and @node_b in the list.
 * Every case complexity: O(1).
 * 
 * If:
 *   - TODO
 * Then:
 *   - @node_a will take @node_b's previous position in the list.
 *   - @node_b will take @node_a's previous position in the list.
 */
static inline void llist_swap(LListNode *node_a, LListNode *node_b)
{
    LListNode *tmp_prev;
    LListNode *tmp_next;

    /* Put @node_b in @node_a's spot. */
    tmp_next = b->next; /* to put a in b's spot */
    tmp_prev = b->prev;
    node_b->prev = a->prev;
    node_b->next = a->next;
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
 * @start_node: Node to start iteration at.
 * @cursor: Points to where iteration currently is.
 */
#define LLIST_FOR_EACH_AFTER(start_node, cursor) \
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
#define LLIST_FOR_EACH_BETWEEN(start_node, end_node, cursor) \
    for (cursor = start_node; cursor != end_node; cursor = cursor->next)

#endif
