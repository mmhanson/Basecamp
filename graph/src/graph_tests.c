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
        TEST_ASSERT_EQUAL(NULL, graph.nodes[idx].edges_out);
    }
}

void test_bucket_init()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    Bucket *bucket;
    int idx;

    graph_init(&graph, node_arr, INIT_SIZE);
    bucket = malloc(sizeof(Bucket));
    graph_add_bucket(&graph, 0, bucket);

    /* Validate bucket attributes. */
    for (idx = 0; idx < BUCKET_SIZE; idx++)
    {
        TEST_ASSERT_EQUAL( NULL, (bucket->adj_nodes)[idx] );
    }
    TEST_ASSERT_EQUAL(NULL, bucket->next);
}

void test_add_first_bucket()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    Bucket *bucket;
    int idx;

    graph_init(&graph, node_arr, INIT_SIZE);
    bucket = malloc(sizeof(Bucket));
    graph_add_bucket(&graph, 0, bucket);

    TEST_ASSERT_EQUAL(bucket, graph.nodes[0].edges_out);
}

void test_add_another_bucket()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    Bucket *bucket0;
    Bucket *bucket1;

    graph_init(&graph, node_arr, INIT_SIZE);
    bucket0 = malloc(sizeof(Bucket));
    bucket1 = malloc(sizeof(Bucket));
    graph_add_bucket(&graph, 0, bucket0);
    graph_add_bucket(&graph, 0, bucket1);

    TEST_ASSERT_EQUAL(bucket0, graph.nodes[0].edges_out);
    TEST_ASSERT_EQUAL(bucket1, graph.nodes[0].edges_out->next);
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

void test_add_edge_no_space()
{
    Graph graph;
    Node node_arr[INIT_SIZE];
    Bucket *bucket;
    int retval;

    graph_init(&graph, node_arr, INIT_SIZE);
    retval = graph_add_edge(&graph, 0, 1);

    TEST_ASSERT_EQUAL(1, retval);
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
        graph_add_bucket(&graph, idx, bucket);
    }

    graph_add_edge(&graph, 0, 1);
    graph_add_edge(&graph, 1, 2);
    graph_add_edge(&graph, 2, 3);
    graph_add_edge(&graph, 3, 4);

    graph_del_edge(&graph, 0, 1);
    graph_del_edge(&graph, 1, 2);

    TEST_ASSERT_EQUAL(1, graph_has_edge(&graph, 0, 1));
    TEST_ASSERT_EQUAL(1, graph_has_edge(&graph, 1, 2));

    TEST_ASSERT_EQUAL(0, graph_has_edge(&graph, 2, 3));
    TEST_ASSERT_EQUAL(0, graph_has_edge(&graph, 3, 4));
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
        graph_add_bucket(&graph, idx, bucket);
    }

    retval = graph_has_edge(&graph, 0, 1);
    TEST_ASSERT_EQUAL(1, retval);

    retval = graph_has_edge(&graph, 2, 4);
    TEST_ASSERT_EQUAL(1, retval);

    retval = graph_has_edge(&graph, 5, 8);
    TEST_ASSERT_EQUAL(1, retval);
}

int main()
{
    UNITY_BEGIN();


    /*   verify initial values of a graph  */
    RUN_TEST(test_init_values);

    /*  add a bucket to an empty graph, verify bucket initialization  */
    RUN_TEST(test_bucket_init);
    /*  add a bucket to an empty graph, verify addition  */
    RUN_TEST(test_add_first_bucket);
    /*  add two buckets to one node  */
    RUN_TEST(test_add_another_bucket);

    /*  add a single edge between first two nodes, verify edge  */
    RUN_TEST(test_basic_add_edge);
    /*  add a double edge between first two ndoes, verify edges  */
    RUN_TEST(test_double_edge);
    /*  add a few edges between nodes  */
    RUN_TEST(test_add_couple_edges);
    /*  add an edge to a node with no space for an edge, verify the error  */
    RUN_TEST(test_add_edge_no_space);

    /*  add a edge and delete it, verify removal  */
    RUN_TEST(test_basic_del_edge);
    /*  add a few edges, delete some but not all, verify results  */
    RUN_TEST(test_del_couple_edges);

    /*  test that an empty graph doesnt have an edges  */
    RUN_TEST(test_has_edge_empty_graph);
    /*  test a basic graph for nonexistent edges  */
    RUN_TEST(test_basic_not_has_edge);


    UNITY_END();
}
