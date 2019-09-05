/*
 * A simple directed graph implemented with an adjacency list.
 *
 *
 *
 * Written by Max Hanson, September 2019.
 * Released into the public domain under CC0. See README.md for more details.
 */

#ifndef GRAPH_H
#define GRAPH_H


// How many edges out per bucket.
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
 * attributes will be updated. @graph.size will equal @node_arr_size,
 * @graph.num_nodes and @graph.num_edges will equal zero. Every node in @graph's
 * node list will receive an id equal to its index (ie 0..n-1) and every node's
 * 'edges_out' pointer will be 0.
 *
 * @graph: The graph to initialize.
 * @node_arr: An array for the graph to keep its nodes in.
 * @node_arr_size: The size of @node_arr.
 */
void graph_init(Graph *graph, Node* node_arr, int node_arr_size)
{

}

/*
 * Initialize a bucket.
 * Makes all attributes of @bucket null.
 *
 * @bucket: The bucket to initialize.
 */
void graph_bucket_init(Bucket *bucket)
{
    
}

/*
 * Add a bucket to a node's edges-out-list.
 *
 * @graph: The graph.
 * @node_id: Id of the node to add @bucket.
 * @bucket: pointer to bucket object to use for @node.
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
void graph_del_edge(Graph *graph, int from_id, int to_id)
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
