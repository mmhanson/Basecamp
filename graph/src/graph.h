/*
 * A simple directed graph implemented with an adjacency list.
 *
 * This header was designed to be basic and easily extendable/modifiable. It was
 * not designed to be the catch-all tool for all your graph needs. This
 * header just serves as a template for the user to custom-fit to their
 * specific use case.
 *
 * === How to Use ===
 * There are two main points:
 *   1. Nodes are accessed by the client via their unique id. All methods that
 *      receive a node's id will assume that it is valid, and undefined behavior
 *      will result if it is not.
 *   2. This header does no memory management. The client will need to provide
 *      two things for the graph to work: space for the graph to keep its nodes,
 *      and space for the graph to keep its edges.
 *      Nodes are kept in an array of nodes which is pointed to by the graph.
 *      Edges are kept in a linked list stemming from each node, in the manor
 *      of an adjacency list. The components of this linked list are called
 *      buckets because they keep a handful of edges. This is done to reduce
 *      indirection during runtime. The client will need to provide nodes with
 *      buckets via the 'add_bucket' operation, otherwise the 'add_edge'
 *      operation will fail.
 *
 * === How to Modify ===
 * This header was desigend to be extensible and basic, not a catch-all graphing
 * library. Below I've written some of the ways I left the header open for
 * extensions.
 *   - Graph nodes are consolidated to their own struct, making it simple to add
 *     or modify information pertaining to individual nodes. Node ids can be
 *     changed to be more meaningful to their application, eg names of cities.
 *     And more information can be added to nodes, eg their degree.
 *   - The graph can be made undirected with a simple extension of the add_edge
 *     operation to add an edge to both nodes in question.
 *   - The graph can be made weighted by recording weights next to edges in the
 *     nodes buckets.
 *   - If graphs are very sparse, an array of edges can be kept in the nodes
 *     themselves, reducing the memory management done by the user. You could
 *     even embed the array of nodes in the graph struct to elminiate the heap
 *     entirely.
 *
 * Written by Max Hanson, September 2019.
 * Released into the public domain under CC0. See README.md for more details.
 */

#ifndef GRAPH_H
#define GRAPH_H

typedef struct BucketTag Bucket;
typedef struct NodeTag Node;
typedef struct GraphTag Graph;

/*  how many edges out per bucket  */
#define BUCKET_SIZE 10

static Node *graph_find_node_by_id(Graph *graph, int node_id);
static Node **graph_find_edge(Graph *graph, int from_id, int to_id);
static Node **graph_find_empty_edge(Graph *graph, int node_id);
static Node **graph_find_pointer(Graph *graph, Node *node_from, Node *node_to);

/*
 * A bucket of edges out of a node.
 *
 * @adj_nodes: Array of edges out of the node.
 * @next: The next bucket. Null if this is the last bucket.
 */
struct BucketTag
{
    Node *adj_nodes[BUCKET_SIZE];
    Bucket *next;
};

/*
 * A node in the graph.
 *
 * @id: The node's unique identifying number. Always positive.
 * @edges_out: Linked list of each edge leaving this node.
 */
struct NodeTag
{
    int id;
    Bucket *edges_out;
};

/*
 * A graph.
 * CAUTION: @num_nodes <= size(@nodes).
 *
 * @size: The maximum number of nodes the graph can hold.
 * @num_nodes: The number of nodes in the graph.
 * @num_edges: The number of edges in the graph.
 * @nodes: A pointer to an array containing the graph's nodes.
 */
struct GraphTag
{
    int size;
    int num_nodes;
    int num_edges;
    Node *nodes;
};

/*
 * Initialize a graph.
 * @graph will be initialized to use @node_arr for its node storage and its
 * attributes will be initialized to:
 *   - size: @node_arr_size
 *   - num_nodes, num_edges: 0
 *   - nodes: Each node in the array will get an id equal to their index and its
 *     edges_out attribute will be null.
 * @graph's previous 'nodes' attribute will not be modified, so this function
 * can also be used to re-initialize a graph to expand/contract it.
 *
 * @node_arr: An array for the graph to keep its nodes in.
 * @node_arr_size: The size of @node_arr.
 */
static void graph_init(Graph *graph, Node* node_arr, int node_arr_size)
{
    int idx;
    
    graph->size = node_arr_size;
    graph->num_nodes = 0;
    graph->num_edges = 0;
    graph->nodes = node_arr;

    for (idx = 0; idx < node_arr_size; idx++)
    {
        node_arr[idx].id = idx;
        node_arr[idx].edges_out = 0;
    }
}

/*
 * Initialize and add a bucket to a node's edge list.
 *
 * @node_id: Id of the node to add @bucket.
 * @bucket: pointer to bucket object to use for @node. Assumed to be just
 *   allocated. All fields will be initialized to null. Will be linked into the
 *   node's list of buckets as the last bucket.
 */
static void graph_add_bucket(Graph *graph, int node_id, Bucket* bucket)
{
    int idx;
    Bucket *cursor;
    Node *node;

    node = graph_find_node_by_id(graph, node_id);

    /*  initialize the bucket  */
    bucket->next = 0;
    for (idx = 0; idx < BUCKET_SIZE; idx++)
    {
        bucket->adj_nodes[idx] = 0;
    }

    /*  start a bucket chain, if one doesnt exist  */
    if (node->edges_out == 0)
    {
        node->edges_out = bucket;
    }
    /*  otherwise, add the bucket to the end of the chain  */
    else
    {
        cursor = node->edges_out;
        while (cursor->next != 0)
        {
            cursor = cursor->next;
        }
        cursor->next = bucket;
    }
}

/*
 * Add an edge to a graph.
 *
 * @from_id: The id of the node the edge starts at. Assumed to be valid.
 * @to_id: The id of the node the edge ends at. Assumed to be valid.
 * @return: 0 if the edge was added, 1 if there wasn't enough space for it.
 */
static int graph_add_edge(Graph *graph, int from_id, int to_id)
{
    Node *node_to;
    Node **edge_spot;

    edge_spot = graph_find_empty_edge(graph, from_id);
    if (edge_spot == 0)
    {
        /*  no empty edges  */
        return 1;
    }

    node_to = graph_find_node_by_id(graph, to_id);
    (*edge_spot) = node_to;

    return 0;
}

/*
 * Remove an edge from a graph.
 *
 * @from_id: The id of the node the edge starts at. Assumed to be valid.
 * @to_id: The id of the node the edge ends at. Assumed to be valid.
 */
static void graph_del_edge(Graph *graph, int from_id, int to_id)
{
    Node **edge;

    /*  find first null pointer in @from_ids list of edges out  */
    edge = graph_find_edge(graph, from_id, to_id);

    if (edge == 0)
    {
        /*  edge doesnt exist, consider it deleted  */
        return;
    }

    /*  delete the edge  */
    (*edge) = 0;
}

/*
 * Determine if there is an edge in a graph.
 *
 * @from_id: The id of the node the edge starts at. Assumed to be valid.
 * @to_id: The id of the node the edge ends at. Assumed to be valid.
 * @return: 0 if the graph has the edge. 1 if it doesnt.
 */
static int graph_has_edge(Graph *graph, int from_id, int to_id)
{
    Node **edge_ptr;

    edge_ptr = graph_find_edge(graph, from_id, to_id);

    if (edge_ptr == 0)
    {
        /*  pointer to the edge is null, there is no such edge  */
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
 * Determine if a node id is valid.
 *
 * @return: Bool. 1 if the node id is valid, 0 if not.
 */
static int graph_node_id_valid(Graph *graph, int node_id)
{
    /*  a nodes id is just its index  */
    if (node_id < 0 || node_id >= graph->size)
    {
        return 1;
    }
    return 0;
}


/* === HELPER FUNCTIONS === */

/*
 * Find a node using its unique identifier.
 *
 * @node_id: The unique identifier for the node. Assumed to be valid.
 * @return: Pointer to the corresponding node.
 * @return: Pointer to the node, or null if the node cannot be found.
 */
static Node *graph_find_node_by_id(Graph *graph, int node_id)
{
    /*  a nodes id is just its index  */
    return &graph->nodes[node_id];
}

/*
 * Find the edge in a graph.
 *
 * @from_id: Id of the node the edge starts at. Assumed to be valid.
 * @to_id: Id of he node the edge ends at. Assumed to be valid.
 * @return:
 *   1. A pointer to the edge. Points to the spot in @from_id's list of edges
 *      out that points to @to_id.
 *   2. Null if the edge doesn't exist or either node id is invalid.
 */
static Node **graph_find_edge(Graph *graph, int from_id, int to_id)
{
    Node *node_to;
    Node *node_from;

    node_to = graph_find_node_by_id(graph, to_id);
    node_from = graph_find_node_by_id(graph, from_id);

    return graph_find_pointer(graph, node_from, node_to);
}

/*
 * Find the first empty spot to put a new edge in a graph.
 *
 * @node_id: Id of the node to find the empty edge in. Assumed to be valid.
 * @return:
 *   1. A pointer to the emtpy edge. Points to the spot in @node_id's list of
 *      edges out that is empty.
 *   2. Null if there is no empty edges
 */
static Node **graph_find_empty_edge(Graph *graph, int node_id)
{
    Node *node;

    node = graph_find_node_by_id(graph, node_id);

    /*  find first null pointer in @node's list of edges out  */
    return graph_find_pointer(graph, node, 0);
}

/*
 * Find a specific pointer in a node's list of edges.
 *
 * @node_from: The node whose list of edges out to find the pointer in. Assumed
 *   to be non-null.
 * @node_to: The target pointer to find in @node_from's list of edges out. Can
 *   be null, in which case it will find the first null edge.
 * @return:
 *   1. A pointer to the spot that points to @node_to.
 *   2. Null, if the pointer doesn't exist.
 */
static Node **graph_find_pointer(Graph *graph, Node *node_from, Node *node_to)
{
    int idx;
    Bucket *cursor;
    Node **adj_nodes;

    /*  iterate through each bucket  */
    cursor = node_from->edges_out;
    while (cursor != 0)
    {
        /*  iterate through each spot in the bucket  */
        for (idx = 0; idx < BUCKET_SIZE; idx++)
        {
            adj_nodes = cursor->adj_nodes;
            if (adj_nodes[idx] == node_to)
            {
                /*  pointer is found  */
                return &(adj_nodes[idx]);
            }
        }

        cursor = cursor->next;
    }

    /*  edge not found  */
    return 0;
}


#endif
