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
    }
}

void test_init_bucket_values()
{
    Bucket *bucket;
    int idx;

    bucket = malloc(sizeof(Bucket));
    graph_bucket_init(bucket);

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

    graph_init(&graph, node_arr, INIT_SIZE);
    bucket = malloc(sizeof(Bucket));
    graph_bucket_init(bucket);
    graph_add_bucket(&graph, 0, bucket);

    TEST_ASSERT_EQUAL(bucket, graph.nodes[0].edges_out);
}

int main()
{
    UNITY_BEGIN();

    /* Init graph. Verify initial values. */
    RUN_TEST(test_init_values);
    /* Init bucket. Verify initial values. */
    RUN_TEST(test_init_bucket_values);
    /* Add a bucket to an empty graph. Verify. */
    RUN_TEST(test_add_first_bucket);


    UNITY_END();
}
