/*
 * A basic circular, doubly linked list.
 *
 * USAGE
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
 * Initialize a linked list node to its default values (circular to itself).
 */
#define LLIST_INIT(container, head_var_name) \
    { container, (&head_var_name), (&head_var_name) }

/*
 * Add @new_node after @node in the list.
 *
 * Every case complexity: BIG-THETA(1).
 *
 * @node: Add @new_node after this node in the list.
 * @new_node: Add this node after @node in the list.
 * @container: The struct which @new_node is contained in. See usage at top.
 */
static inline void llist_add_after(LListNode *node, LListNode *new_node,
                                   void *new_node_container)
{
    new_node->container = new_node_container;

    new_node->next = node->next;
    new_node->prev = node;
    node->next->prev = new_node;
    node->next = new_node;
}

/*
 * Add @new_node before @node in the list.
 *
 * Every case complexity: BIG-THETA(1).
 *
 * @node: Add @new_node before this node in the list.
 * @new_node: Add this node before @node in the list.
 * @container: The struct which @new_node is contained in. See usage at top.
 */
static inline void llist_add_before(LListNode *node, LListNode *new_node,
                                    void *new_node_container)
{
    new_node->container = new_node_container;

    new_node->next = node;
    new_node->prev = node->prev;
    node->prev->next = new_node;
    node->prev = new_node;
}



#endif
