/*
 * A simple directed graph implemented with an adjacency list.
 *
 * This header was designed to be basic and easily extendable/modifiable. It was
 * not designed to be the catch-all tool for all your graph needs. This
 * header just serves as a template for the user to custom-fit to their
 * specific use case.
 *
 * === How to Use ===
 * The use of this header is fairly straightforward except for the memory
 * allocation. This header performs no memory allocation, so the user is
 * required to allocate and provide the structs for these operations to work
 * with.
 * The nodes in the graph use a linked-list-like structure to keep their edges
 * out. Instead of having one link for each edge out, they are grouped in
 * 'buckets' of a handful of edges and then those buckets are linked together.
 * This is done to alleviate the strain on the heap and to keep the user from
 * having to allocate a new link each time an edge is made.
 * The user will need to allocate these buckets and add them to nodes with the
 * 'add_bucket' operation if there is no space for an edge to be added to a
 * node. The nodes of a newly-initialized graph have no buckets.
 *
 * Written by Max Hanson, September 2019.
 * Released into the public domain under CC0. See README.md for more details.
 */

#ifndef GRAPH_H
#define GRAPH_H


/* How many edges out per bucket. */
#define BUCKET_SIZE 10

typedef struct BucketTag Bucket;
typedef struct NodeTag Node;
typedef struct GraphTag Graph;

/*
 * A bucket of edges out of a vertex.
 * Each node in the graph has a linked list of outgoing edges. These edges are
 * stored in 'buckets' of a few edges out (the exact number defined in
 * BUCKET_SIZE) and links to the next. This reduces indirection and the load on
 * the heap.
 *
 * @adj_nodes: Edges out of the node.
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
 * attributes will be initialized:
 *   size: @node_arr_size
 *   num_nodes, num_edges: 0
 *   nodes: Each node in the array will get an id equal to their index and its
 *     edges_out attribute will be null.
 * @graph's previous 'nodes' attribute will not be modified, so this function
 * can be used to re-initialize a graph to expand/contract it.
 *
 * @graph: The graph to initialize.
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
 * Initialize and add a bucket to a node's edges-out-list.
 *
 * @graph: The graph.
 * @node_id: Id of the node to add @bucket.
 * @bucket: pointer to bucket object to use for @node. Assumed to be just
 *   allocated. All fields will be initialized to null. Will be linked into the
 *   node's list of buckets as the last bucket.
 * @return: 0 if the bucket was added. -1 if the node could not be found.
 */
static int graph_add_bucket(Graph *graph, int node_id, Bucket* bucket)
{
    int idx;
    Bucket **link_cursor;
    Node *node; /* Node to add bucket to. */

    node = graph_get_node_by_id(graph, node_id);
    if (node == 0)
    {
        return -1;
    }

    bucket->next = 0;
    for (idx = 0; idx < BUCKET_SIZE; idx++)
    {
        bucket->adj_nodes[idx] = 0;
    }

    /* Find the last link (Bucket*) of the node's bucket chain. */
    link_cursor = &(node->edges_out);
    while ( (*link_cursor) != 0 )
    {
        link_cursor = (*link_cursor)->next;
    }
    (*link_cursor) = bucket; /* Add new bucket onto chain. */

    return 0;
}

/*
 * Add an edge to a graph.
 *
 * @graph: the graph to add the edge to.
 * @from_id: the id of the node the edge starts at.
 * @to_id: the id of the node the edge ends at.
 * @return: 0 if the edge was added, -1 if there wasn't enough space for it,
 *   and -2 if either node id is invalid.
 */
static int graph_add_edge(Graph *graph, int from_id, int to_id)
{
    Node *node_to;
    Node **edge;
    int status;

    /* Find first null pointer in @from_ids list of edges out. */
    status = graph_find_edge(graph, from_id, 0, edge);
    if (status == 1)
    {
        return -1; /* No space for a new edge. */
    }
    if (status == -1)
    {
        return -2; /* @from_id is invalid. */
    }

    /* Find what to point to edge to. */
    node_to = graph_get_node_by_id(graph, to_id);
    if (node_to == 0)
    {
        return -2; /* @to_id is invald. */
    }

    (*edge) = node; /* Add the edge. */
    return 0;
}

/*
 * Remove an edge from a graph.
 *
 * @graph: the graph to remove the edge from.
 * @from_id: the id of the node the edge starts at.
 * @to_id: the id of the node the edge ends at.
 * @return: 0 if the edge was deleted. 1 if the edge does not exist. -1 if
 *   either node id is invalid.
 */
static int graph_del_edge(Graph *graph, int from_id, int to_id)
{
    Node **edge;
    int status;

    /* Find first null pointer in @from_ids list of edges out. */
    status = graph_find_edge(graph, from_id, to_id, edge);
    if (status == 1)
    {
        return 1; /* Edge does not exist. */
    }
    if (status == -1)
    {
        return -1; /* A node is invalid. */
    }

    (*edge) = 0; /* Delete the edge. */
    return 0;
}

/*
 * Determine if there is an edge in a graph.
 *
 * @graph: the graph to check for the edge.
 * @from_id: the id of the node the edge starts at.
 * @to_id: the id of the node the edge ends at.
 * @return: 0 if the graph has the edge. 1 if not. -1 if either node is is
 *   invalid.
 */
static int graph_has_edge(Graph *graph, int from_id, int to_id)
{
    Node **edge;

    /* Return statuses match up. */
    return graph_find_edge(graph, from_id, to_id, edge);
}


/* === HELPER FUNCTIONS === */

/*
 * Get a node using its unique identifier.
 * Modify this method if you want to change how nodes are identified.
 *
 * @graph: The graph with the node to find.
 * @node_id: The unique identifier for the node.
 * @return: Pointer to the node, or null (0) if the node cannot be found.
 */
static Node *graph_get_node_by_id(Graph *graph, int node_id)
{
    /* A nodes id is just its index. */
    if (node_id < 0 || node_id >= graph->size)
    {
        return 0;
    }

    return &graph->nodes[node_id];
}

/*
 * Find the edge in a graph.
 * Note that both @from_id must be a valid id. @to_id must either be a valid id
 * or -1 (see below).
 *
 * @graph: The graph to find the edge in.
 * @from_id: Id of the node the edge starts at. Must be a valid node id.
 * @to_id: Id of he node the edge ends at. Must be a valid node id or -1.
 *   Provide a value of -1 to find the first empty spot to put a new edge in the
 *   graph. Will return 1 if there is no empty spots to put a new edge in.
 * @edge_ptr: Will point to the found edge if @return is 0. Otherwise
 *   unmodified. This will point to the spot in @from_id's list of edges out
 *   that points to @to_id.
 * @return: 0 if the edge was found and @edge_ptr set. 1 if the node is not
 *   found and -1 if either node id is invalid.
 */
static int graph_find_edge(Graph *graph, int from_id, int to_id, Node **edge_ptr)
{
    Node *node_to;
    Node *node_from;

    node_from = graph_get_node_by_id(graph, from_id);
    if (node_from == 0)
    {
        return -1; /* Invalid @from_id. */
    }

    if (to_id == -1)
    {
        node_to = 0; /* Looking for empty spot in @node_from. */
    }
    else
    {
        /* Looking for an actual edge. */
        node_to = graph_get_node_by_id(graph, to_id);
        if (node_to == 0)
        {
            return -1; /* Invalid @to_id. */
        }
    }

    cursor = node_from->edges_out;
    /* Iterate through each bucket. */
    while (cursor != 0)
    {
        /* Iterate through each spot in the bucket. */
        for (idx = 0; idx < BUCKET_SIZE; idx++)
        {
            if (cursor->adj_nodes[idx] == node_to)
            {
                edge_ptr = &(cursor->adj_nodes[idx]);
                return 0;
            }
        }

        /* Go to next bucket. */
        cursor = cursor->next;
    }
    /* Edge not found. */
    return 1;
}


#endif
