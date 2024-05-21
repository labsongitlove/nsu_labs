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


// first dfs for add stack of vertex
Edges ** dfs(Graph **graph, Edge **edges, Edge **edge, int ind, int len){
    (*edge)->visited = 1;

    for (int i = 0; i < len; i++) {
        if (*((*edge)->ver_lst) == (*((*edges) + i))->ver_lst + 1 &&
            (*((*edges) + i))->visited == 0) {
            dfs(graph, edges, *((*edges) + i), i, len);
        }
    }
    add_ver(graph, ind);
    return edges;
}

// second dfs for paint

Edges ** dfs2(Graph **graph, Edge **edges, Edge **edge, int color, int len){
    (*edge)->visited = color;

    for (int i = 0; i < len; i++){
        if (*((*edge)->ver_lst + 1) == *((*((*edges) + i))->ver_lst) &&
                (*edge)->ver_lst != (*((*edges) + i))->ver_lst &&

            (*((*edges) + i))-> visited <= 1)
            dfs2(graph, edges, (*((*edges) + i)), color, len);
    }
    return edges;
}
