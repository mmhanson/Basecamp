/*
 * A basic example of using the graph header to create a meaningless graph.
 *
 * Written by Max Hanson, September 2019.
 * Released into the public domain under CC0. See README.md for more details.
 */

#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

#define INIT_SIZE 20


void graph_double(Graph *graph);

int main()
{
    Graph graph;
    Node *graph_nodes;
    int edge_cnt, node_from, node_to;

    /* This graph initially supports INIT_SIZE nodes. */
    graph_nodes = malloc(INIT_SIZE * sizeof(Node));
    /* Set graph attrs and make graph use 'graph_nodes' to store its nodes. */
    graph_init(&graph, graph_nodes, INIT_SIZE);

    /* Randomly add 5 edges between the nodes. */
    for (edge_cnt = 0; edge_cnt < 5; edge_cnt++)
    {
        node_from = rand() % INIT_SIZE;
        node_to = rand() % INIT_SIZE;

        graph_add_edge(&graph, node_from, node_to);
    }

    /* Expand the graph to include 2 * INIT_SIZE nodes. */
    graph_double(&graph);

    /* Randomly add 5 more edges between the nodes. */
    for (edge_cnt = 0; edge_cnt < 5; edge_cnt++)
    {
        node_from = rand() % (INIT_SIZE * 2);
        node_to = rand() % (INIT_SIZE * 2);

        graph_add_edge(&graph, node_from, node_to);
    }
}

/*
 * Make a new array that doubles the capacity of the graph and copy the nodes.
 *
 * @graph: The graph to double.
 */
void graph_double(Graph *graph)
{
    Node *new_graph_nodes;
    Node *old_graph_nodes;
    int idx;
    int new_size;
    int old_size;

    old_size = graph->size;
    new_size = 2 * graph->size;
    new_graph_nodes = malloc(new_size * sizeof(Node));

    old_graph_nodes = graph->nodes; /* Save old nodes to copy over. */
    /* Reinitialize graph for new node array. */
    graph_init(graph, new_graph_nodes, new_size);
    for (idx = 0; idx < old_size; idx++)
    {
        /* Point new nodes to old nodes buckets to copy the edges. */
        new_graph_nodes[idx].edges_out = old_graph_nodes[idx].edges_out;
    }
}
