#include <stdlib.h>
#pragma once


typedef struct Graph
{
    int key;
    long int value;
    struct Graph* next;
}Graph;


typedef struct Adj
{
    int key;
    struct Graph** adj_lst;
}Adj;


//steps in dijkstra alg
typedef struct Dijstp
{
    int key; //key of past vertex
    int done; //bool, have you been there
    long int dist; //the distance from the first to the current vertex
}Dijstp;


Adj* create_adj(int key)
{
    Adj* adj = (Adj*)malloc(sizeof(Adj));
    adj->key = key;
    adj->adj_lst = (Graph**)malloc(key * sizeof(Graph*));

    int num = 0;
    while (num < key)
        adj->adj_lst[num++] = NULL;

    return adj;
}


Graph* create_graph(int key, long int value)
{
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->key = key;
    graph->value = value;
    graph->next = NULL;
    return graph;
}


void add_edge(Adj* adj, int *lst, long int value)
{
    for (int i = 0; i < 2; i++){
        Graph* graph = create_graph(lst[(i + 1) % 2], value);
        graph->next = *(adj->adj_lst + *(lst + i) - 1);
        *(adj->adj_lst + *(lst + i) - 1) = graph;
    }
}