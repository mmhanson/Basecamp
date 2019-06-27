/*
 * MIT License
 *
 * Copyright (c) 2019 Max Hanson
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/*
 * A basic doubly linked list.
 *
 * === How To Use ===
 * This header defines a struct representing a linked list node (LListNode). All
 * linked list operations operate on these nodes. Instances of a struct can be
 * linked together by embedding a LListNode in the struct and managing the
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
 * O(1) complexity. @new_node doesnt need to be initialized, its members will be
 * overwritten anyway.
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
 * O(1) complexity. @new_node doesnt need to be initialized, its members will be
 * overwritten anyway.
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
 * O(1) complexity. Caution! @node will not be deallocated.
 *
 * @node: The node to be removed from the list.
 */
static inline void llist_remove(LListNode *node)
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
static inline void llist_replace(LListNode *old_node, LListNode *new_node)
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
