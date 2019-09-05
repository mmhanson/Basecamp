/*
 * Unit tests for the graph header.
 *
 * Written by Max Hanson, September 2019.
 * Released into the public domain under CC0. See README.md for more details.
 */

#include "graph.h"
#include "../../deps/unity/unity.h"
#include <stdlib.h>

#define INIT_SIZE 10


void test_init_values()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    int idx;

    graph_init(&graph, node_arr, INIT_SIZE);

    TEST_ASSERT_EQUAL(INIT_SIZE, graph.size);
    TEST_ASSERT_EQUAL(0, graph.num_nodes);
    TEST_ASSERT_EQUAL(0, graph.num_edges);
    TEST_ASSERT_EQUAL(&node_arr, graph.nodes);
    for (idx = 0; idx < INIT_SIZE; idx++)
    {
        TEST_ASSERT_EQUAL(idx, graph.nodes[idx].id);
        TEST_ASSERT_EQUAL(Null, graph.nodes[idx].edges_out);
    }
}

void test_add_first_bucket()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    Bucket *bucket;
    int retval;

    graph_init(&graph, node_arr, INIT_SIZE);
    bucket = malloc(sizeof(Bucket));
    graph_bucket_init(bucket);
    retval = graph_add_bucket(&graph, 0, bucket);

    TEST_ASSERT_EQUAL(0, retval);
    TEST_ASSERT_EQUAL(bucket, graph.nodes[0].edges_out);
    /* Validate bucket attributes. */
    for (idx = 0; idx < BUCKET_SIZE; idx++)
    {
        TEST_ASSERT_EQUAL( NULL, (bucket->adj_nodes)[idx] );
    }
    TEST_ASSERT_EQUAL(NULL, bucket->next);
}

void test_add_another_bucket()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    Bucket *bucket0;
    Bucket *bucket1;
    int retval0;
    int retval1;

    graph_init(&graph, node_arr, INIT_SIZE);
    bucket0 = malloc(sizeof(Bucket));
    bucket1 = malloc(sizeof(Bucket));
    graph_bucket_init(bucket0);
    graph_bucket_init(bucket1);
    retval0 = graph_add_bucket(&graph, 0, bucket0);
    retval1 = graph_add_bucket(&graph, 0, bucket1);

    TEST_ASSERT_EQUAL(0, retval0);
    TEST_ASSERT_EQUAL(0, retval1);
    TEST_ASSERT_EQUAL(bucket0, graph.nodes[0].edges_out);
    TEST_ASSERT_EQUAL(bucket1, graph.nodes[0].edges_out->next);
}

void test_add_bucket_invalid_node()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    Bucket *bucket;
    int retval;

    graph_init(&graph, node_arr, INIT_SIZE);
    bucket = malloc(sizeof(Bucket));
    graph_bucket_init(bucket);
    retval = graph_add_bucket(&graph, INIT_SIZE + 1, bucket);

    TEST_ASSERT_EQUAL(1, retval);
}

void test_basic_add_edge()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    Bucket *bucket;
    int idx;

    /* Init graph with one bucket for each node */
    graph_init(&graph, node_arr, INIT_SIZE);
    for (idx = 0; idx < INIT_SIZE; idx++)
    {
        bucket = malloc(sizeof(Bucket));
        graph_bucket_init(bucket);
        graph_add_bucket(&graph, idx, bucket);
    }
    graph_add_edge(&graph, 0, 1);

    TEST_ASSERT_EQUAL(0, graph_has_edge(&graph, 0, 1));
    TEST_ASSERT_EQUAL(1, graph_has_edge(&graph, 1, 0));
}

void test_double_edge()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    Bucket *bucket;
    int idx;

    /* Init graph with one bucket for each node */
    graph_init(&graph, node_arr, INIT_SIZE);
    for (idx = 0; idx < INIT_SIZE; idx++)
    {
        bucket = malloc(sizeof(Bucket));
        graph_bucket_init(bucket);
        graph_add_bucket(&graph, idx, bucket);
    }
    graph_add_edge(&graph, 0, 1);
    graph_add_edge(&graph, 1, 0);

    TEST_ASSERT_EQUAL(0, graph_has_edge(&graph, 0, 1));
    TEST_ASSERT_EQUAL(0, graph_has_edge(&graph, 1, 0));
}

void test_add_couple_edges()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    Bucket *bucket;
    int idx;

    /* Init graph with one bucket for each node */
    graph_init(&graph, node_arr, INIT_SIZE);
    for (idx = 0; idx < INIT_SIZE; idx++)
    {
        bucket = malloc(sizeof(Bucket));
        graph_bucket_init(bucket);
        graph_add_bucket(&graph, idx, bucket);
    }
    graph_add_edge(&graph, 0, 1);
    graph_add_edge(&graph, 2, 4);
    graph_add_edge(&graph, 5, 9);
    graph_add_edge(&graph, 8, 7);

    TEST_ASSERT_EQUAL(0, graph_has_edge(&graph, 0, 1));
    TEST_ASSERT_EQUAL(0, graph_has_edge(&graph, 2, 4));
    TEST_ASSERT_EQUAL(0, graph_has_edge(&graph, 5, 9));
    TEST_ASSERT_EQUAL(0, graph_has_edge(&graph, 8, 7));

    TEST_ASSERT_EQUAL(1, graph_has_edge(&graph, 1, 0));
    TEST_ASSERT_EQUAL(1, graph_has_edge(&graph, 4, 2));
    TEST_ASSERT_EQUAL(1, graph_has_edge(&graph, 9, 5));
    TEST_ASSERT_EQUAL(1, graph_has_edge(&graph, 7, 8));

    TEST_ASSERT_EQUAL(1, graph_has_edge(&graph, 3, 2));
    TEST_ASSERT_EQUAL(1, graph_has_edge(&graph, 6, 5));
    TEST_ASSERT_EQUAL(1, graph_has_edge(&graph, 9, 8));
}

void test_add_invalid_edges()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    Bucket *bucket;
    int idx;
    int retval;

    /* Init graph with one bucket for each node */
    graph_init(&graph, node_arr, INIT_SIZE);
    for (idx = 0; idx < INIT_SIZE; idx++)
    {
        bucket = malloc(sizeof(Bucket));
        graph_bucket_init(bucket);
        graph_add_bucket(&graph, idx, bucket);
    }

    /* Second node invalid. */
    retval = graph_add_edge(&graph, 0, INIT_SIZE + 1);
    TEST_ASSERT_EQUAL(-2, retval);

    /* First node invalid. */
    retval = graph_add_edge(&graph, INIT_SIZE + 1, 0);
    TEST_ASSERT_EQUAL(-2, retval);

    /* Both nodes invalid. */
    retval = graph_add_edge(&graph, -5, INIT_SIZE + 1);
    TEST_ASSERT_EQUAL(-2, retval);
}

void test_add_edge_no_space()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    Bucket *bucket;
    int retval;

    graph_init(&graph, node_arr, INIT_SIZE);
    retval = graph_add_edge(&graph, 0, 1);

    TEST_ASSERT_EQUAL(-1, retval);
}

void test_basic_del_edge()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    Bucket *bucket;
    int idx;
    int retval;

    /* Init graph with one bucket for each node */
    graph_init(&graph, node_arr, INIT_SIZE);
    for (idx = 0; idx < INIT_SIZE; idx++)
    {
        bucket = malloc(sizeof(Bucket));
        graph_bucket_init(bucket);
        graph_add_bucket(&graph, idx, bucket);
    }

    graph_add_edge(&graph, 0, 1);
    graph_del_edge(&graph, 0, 1);

    TEST_ASSERT_EQUAL(1, graph_has_edge(&graph, 0, 1));
}

void test_del_couple_edges()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    Bucket *bucket;
    int idx;
    int retval;

    /* Init graph with one bucket for each node */
    graph_init(&graph, node_arr, INIT_SIZE);
    for (idx = 0; idx < INIT_SIZE; idx++)
    {
        bucket = malloc(sizeof(Bucket));
        graph_bucket_init(bucket);
        graph_add_bucket(&graph, idx, bucket);
    }

    graph_add_edge(&graph, 0, 1);
    graph_add_edge(&graph, 1, 2);
    graph_add_edge(&graph, 2, 3);
    graph_add_edge(&graph, 3, 4);

    graph_del_edge(&graph, 0, 1);
    graph_add_edge(&graph, 1, 2);

    TEST_ASSERT_EQUAL(1, graph_has_edge(&graph, 0, 1));
    TEST_ASSERT_EQUAL(1, graph_has_edge(&graph, 1, 2));

    TEST_ASSERT_EQUAL(0, graph_has_edge(&graph, 2, 3));
    TEST_ASSERT_EQUAL(0, graph_has_edge(&graph, 3, 4));
}

void test_del_nonexistent_edge()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    Bucket *bucket;
    int idx;
    int retval;

    /* Init graph with one bucket for each node */
    graph_init(&graph, node_arr, INIT_SIZE);
    for (idx = 0; idx < INIT_SIZE; idx++)
    {
        bucket = malloc(sizeof(Bucket));
        graph_bucket_init(bucket);
        graph_add_bucket(&graph, idx, bucket);
    }

    retval = graph_del_edge(&graph, 0, 1);

    TEST_ASSERT_EQUAL(1, retval);
}

void test_del_invalid_edges()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    Bucket *bucket;
    int idx;
    int retval;

    /* Init graph with one bucket for each node */
    graph_init(&graph, node_arr, INIT_SIZE);
    for (idx = 0; idx < INIT_SIZE; idx++)
    {
        bucket = malloc(sizeof(Bucket));
        graph_bucket_init(bucket);
        graph_add_bucket(&graph, idx, bucket);
    }

    /* First node invalid. */
    retval = graph_del_edge(&graph, INIT_SIZE + 1, 0);
    TEST_ASSERT_EQUAL(-1, retval);
    
    /* Second node invalid. */
    retval = graph_del_edge(&graph, 0, INIT_SIZE + 1);
    TEST_ASSERT_EQUAL(-1, retval);

    /* Both node invalid. */
    retval = graph_del_edge(&graph, INIT_SIZE + 1, -1);
    TEST_ASSERT_EQUAL(-1, retval);
}

void test_del_edges_populated_nodes()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    Bucket *bucket;
    int idx;
    int retval;

    /* Init graph with one bucket for each node */
    graph_init(&graph, node_arr, INIT_SIZE);
    for (idx = 0; idx < INIT_SIZE; idx++)
    {
        bucket = malloc(sizeof(Bucket));
        graph_bucket_init(bucket);
        graph_add_bucket(&graph, idx, bucket);
    }

    graph_add_edge(&graph, 0, 1);
    graph_add_edge(&graph, 8, 9);
    graph_add_edge(&graph, 7, 2);
    graph_add_edge(&graph, 2, 3);
    graph_add_edge(&graph, 4, 6);
    graph_add_edge(&graph, 4, 5);

    retval = graph_del_edge(&graph, 3, 4);
    TEST_ASSERT_EQUAL(1, retval);

    retval = graph_del_edge(&graph, 6, 8);
    TEST_ASSERT_EQUAL(1, retval);

    retval = graph_del_edge(&graph, 2, 4);
    TEST_ASSERT_EQUAL(1, retval);
}

void test_has_edge_empty_graph()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    Bucket *bucket;
    int idx;
    int retval;

    graph_init(&graph, node_arr, INIT_SIZE);
    retval = graph_has_edge(&graph, 0, 1);

    TEST_ASSERT_EQUAL(1, retval);
}

void test_basic_not_has_edge()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    Bucket *bucket;
    int idx;
    int retval;

    /* Init graph with one bucket for each node */
    graph_init(&graph, node_arr, INIT_SIZE);
    for (idx = 0; idx < INIT_SIZE; idx++)
    {
        bucket = malloc(sizeof(Bucket));
        graph_bucket_init(bucket);
        graph_add_bucket(&graph, idx, bucket);
    }

    retval = graph_has_edge(&graph, 0, 1);
    TEST_ASSERT_EQUAL(1, retval);

    retval = graph_has_edge(&graph, 2, 4);
    TEST_ASSERT_EQUAL(1, retval);

    retval = graph_has_edge(&graph, 5, 8);
    TEST_ASSERT_EQUAL(1, retval);
}

void test_has_edge_invalid_nodes()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    Bucket *bucket;
    int idx;
    int retval;

    /* Init graph with one bucket for each node */
    graph_init(&graph, node_arr, INIT_SIZE);
    for (idx = 0; idx < INIT_SIZE; idx++)
    {
        bucket = malloc(sizeof(Bucket));
        graph_bucket_init(bucket);
        graph_add_bucket(&graph, idx, bucket);
    }

    /* Second invalid. */
    retval = graph_has_edge(&graph, 0, INIT_SIZE + 1);
    TEST_ASSERT_EQUAL(1, retval);

    /* First invalid */
    retval = graph_has_edge(&graph, INIT_SIZE + 1, 4);
    TEST_ASSERT_EQUAL(1, retval);

    /* Both invalid. */
    retval = graph_has_edge(&graph, -1, INIT_SIZE + 1);
    TEST_ASSERT_EQUAL(1, retval);
}

int main()
{
    UNITY_BEGIN();


    /* Init graph. Verify initial values. */
    RUN_TEST(test_init_values);

    /* Add a bucket to an empty graph. Verify. Verify attr init'n of bucket. */
    RUN_TEST(test_add_first_bucket);
    /* Add two buckets to one node. Verify. */
    RUN_TEST(test_add_another_bucket);
    /* Add a bucket to a nonexistent node. Verify error retval. */
    RUN_TEST(test_add_bucket_invalid_node);

    /* Add a single edge between 0th and 1st nodes. Verify. */
    RUN_TEST(test_basic_add_edge);
    /* Add a double edge between two nodes. Verify. */
    RUN_TEST(test_double_edge);
    /* Add a few edges between nodes. Verify. */
    RUN_TEST(test_add_couple_edges);
    /* Test adding invalid edges of all kinds. Verify retvals. */
    RUN_TEST(test_add_invalid_edges);
    /* Test adding an edge to a node with no bucket space. Verify error. */
    RUN_TEST(test_add_edge_no_space);

    /* Add an edge and delete it. Verify removal. */
    RUN_TEST(test_basic_del_edge);
    /* Add some edges. Delete some edges (not all). Verify. */
    RUN_TEST(test_del_couple_edges);
    /* Delete an edge that doesn't exist. Verify error. */
    RUN_TEST(test_del_nonexistent_edge);
    /* Try to delete an edge between invalid node ids. Verify errors. */
    RUN_TEST(test_del_invalid_edges);
    /* Delete edges from nodes that have edges to other nodes, but not to the
       node we're trying to delete the edge from. Verify. */
    RUN_TEST(test_del_edges_populated_nodes);

    /* Test if an empty graph (nodes but no buckets) has an edge. */
    RUN_TEST(test_graph_has_edge_empty_graph);
    /* Test a nonempty graph (nodes have buckets) for nonexistent edges. */
    RUN_TEST(test_basic_not_has_edge);
    /* Test a nonemtpy graph for edges of invalid nodes. */
    RUN_TEST(test_has_edge_invalid_nodes);


    UNITY_END();
}
