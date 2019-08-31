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

/*
 * A bucket of edges out of a vertex.
 * Each node in the graph has a linked list of outgoing edges. These edges are
 * stored in 'buckets' of a few edges out (the exact number defined in
 * BUCKET_SIZE) and links to the next. This reduces pointer-following and reduces
 * the load on the heap.
 *
 * @adjNodes: Edges out of the node.
 * @next: The next bucket. Null if this is the last bucket.
 */
typedef struct BucketTag Bucket;
struct BucketTag
{
    Node *adjNodes[BUCKET_SIZE];
    Bucket *next;
};

/*
 * A node in the graph.
 *
 * @id: The node's unique identifying number. Always positive.
 * @edges_out: Linked list of each edge leaving this node.
 */
typedef struct NodeTag
{
    int id;
    Bucket* edges_out;
} Node;

/*
 * A graph.
 * CAUTION: @num_nodes <= size(@nodes).
 *
 * @num_nodes: The number of nodes in the graph.
 * @num_edges: The number of edges in the graph.
 * @nodes: A pointer to an array containing the graph's nodes.
 */
typedef struct GraphTag
{
    int num_nodes;
    int num_edges;
    Node *nodes;
} Graph;

/*
 * Add an edge to a graph.
 *
 * @graph: the graph to remove the edge from.
 * @from_id: the id of the node the edge starts at.
 * @to_id: the id of the node the edge ends at.
 * @return: 0 if the edge was added, 1 if it could not be.
 */
int add_edge(Graph *graph, int from_id, int to_id)
{

}

/*
 * Add a node to a graph.
 *
 * @graph: the graph to add the node to.
 * @bucket: pointer to the bucket the node will use for storing edges out.
 * @return: the id of the node (a positive number) if successful, -1 if the node
 *          couldn't be added (ie @graph's array of nodes is full).
 */
int add_node(Graph *graph, Bucket *bucket)
{
    
}

/*
 * Remove an edge from a graph.
 *
 * @graph: the graph to remove the edge from.
 * @from_id: the id of the node the edge starts at.
 * @to_id: the id of the node the edge ends at.
 */
void rem_edge(Graph *graph, int from_id, int to_id)
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
int has_edge(Graph *graph, int from_id, int to_id)
{
    
}

/*
 * Determine if a node exists in a graph.
 *
 * @graph: the graph to check for the node.
 * @id: the id of the node to add.
 * @return: 0 if the graph has the node, 1 if not.
 */
int has_node(Graph *graph, int id)
{
    
}


#endif
