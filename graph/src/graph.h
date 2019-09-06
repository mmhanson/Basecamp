/*
 * A simple directed graph implemented with an adjacency list.
 *
 * This header was designed to be basic and easily extendable/modifiable. It was
 * not designed to be the catch-all tool for all your graphing needs. In fact,
 * I think most use cases will require some modification to this header, this
 * header just serves as a starting point.
 *
 * === How to Use ===
 * The use of this header is fairly straightforward except for the memory
 * allocation. This header performs no memory allocation, so the user is
 * required to allocate and provide the structs for these operations to work
 * with.
 * The nodes in the graph use a linked-list-like structure to keep their edges
 * out. Instead of having one link for each edge out, they are grouped in
 * 'buckets' of a handful of edges and then those buckets are linked together.
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
    Bucket* edges_out;
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
void graph_init(Graph *graph, Node* node_arr, int node_arr_size)
{

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
int graph_add_bucket(Graph *graph, int node_id, Bucket* bucket)
{
    
}

/*
 * Add an edge to a graph.
 *
 * @graph: the graph to remove the edge from.
 * @from_id: the id of the node the edge starts at.
 * @to_id: the id of the node the edge ends at.
 * @return: 0 if the edge was added, -1 if there wasn't enough space for it,
 *   and -2 if either node doesn't exist.
 */
int graph_add_edge(Graph *graph, int from_id, int to_id)
{

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
int graph_del_edge(Graph *graph, int from_id, int to_id)
{
    
}

/*
 * Determine if there is an edge in a graph.
 *
 * @graph: the graph to check for the edge.
 * @from_id: the id of the node the edge starts at.
 * @to_id: the id of the node the edge ends at.
 * @return: 0 if the graph has the edge. 1 if not.
 */
int graph_has_edge(Graph *graph, int from_id, int to_id)
{
    
}


#endif
