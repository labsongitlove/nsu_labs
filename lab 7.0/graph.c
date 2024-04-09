#include <stdlib.h>
#pragma once


typedef struct Edge
{
    int visited;
    int *ver_lst;
}Edge;


typedef struct Graph{
    int len;
    int *ver_lst;
}Graph;


Edge * create_edge(int *ver_lst)
{
    Edge *edge = (Edge *)malloc(sizeof(Edge));
    edge->visited = 0;
    edge->ver_lst = ver_lst;
    return edge;
}


Graph * create_graph(int ver_lst)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->len = 0;
    *graph->ver_lst = ver_lst;
    return graph;
}


//add vertex
void add_ver(Graph **graph, int ver){
    *((*graph)->ver_lst + (*graph)->len++) = ver;
}


void print_graph(Graph *graph){
    for (int i = graph->len - 1; i >= 0; i--)
        printf("%d ", *(graph->ver_lst + i));
}


int dfs(Graph **graph, Edge ***edges, Edge *edge, int bypass, int len){
    int cicled = 0;
    int added = 0;
    edge->visited = bypass;
    for (int i = 0; i < len; i++)
        if (*(edge->ver_lst + 1) == *((*((*edges) + i))->ver_lst) &&
            edge->ver_lst != (*((*edges) + i))->ver_lst) {
            if (((*((*edges) + i))->visited) == 0) {
                cicled = dfs(graph, edges, *((*edges) + i), bypass, len);
            }
            else if (((*((*edges) + i))->visited) != edge->visited)
                added = 1;
            else
                return 1;
        }
    if (added == 0)
        add_ver(graph, *(edge->ver_lst + 1));

    return cicled;
}