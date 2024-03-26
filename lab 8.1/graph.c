#include <stdlib.h>
#pragma once


typedef struct Edge
{
    long int value;
    int *ver_lst;
}Edge;


//cluster
typedef struct Clust{
    int len;
    int *ver_lst;
    int *edg_lst;
}Clust;


Edge * create_edge(long int value, int *ver_lst)
{
    Edge *edge = (Edge *)malloc(sizeof(Edge));
    edge->value = value;
    edge->ver_lst = ver_lst;
    return edge;
}


Clust * create_clust(int *ver_lst)
{
    Clust *clust = (Clust *)malloc(sizeof(Clust));
    clust->len = 2;
    clust->ver_lst = ver_lst;
    clust->edg_lst = (int *)malloc(sizeof (int) * 16);
    *clust->edg_lst = *ver_lst;
    *(clust->edg_lst + 1) = *(ver_lst + 1);
    return clust;
}


//add vertex
Clust * add_ver(Clust *clust, int ver, int *edg_lst){
    clust->len++;
    clust->ver_lst = (int *) realloc(clust->ver_lst, sizeof(int) * clust->len);
    clust->edg_lst = (int *) realloc(clust->edg_lst, sizeof(int) * clust->len * 2);
    *(clust->ver_lst + clust->len - 1) = ver;
    *(clust->edg_lst + clust->len * 2 - 4) = *edg_lst;
    *(clust->edg_lst + clust->len * 2 - 3) = *(edg_lst + 1);
    return clust;
}