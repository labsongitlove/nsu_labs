#include "stdio.h"
#include "stdlib.h"

#include "graph.c"


//size checker
int size_chk(int ver_len, int edge_len)
{
    int test = 0;

    if (ver_len < 0 || ver_len > 5000)
    {
        printf("bad number of vertices");
        test = 1;
    }
    else if (edge_len < 0 || edge_len > ver_len * (ver_len - 1) / 2)
    {
        printf("bad number of edges");
        test = 1;
    }
    else if (edge_len == 0 && ver_len == 1)
        test = 1;
    return test;
}


void top_sort(Graph **graph, Edge ***edges, Edge *edge, int bypass, int vert_len, int edge_len){
    int cicled = dfs(graph, edges, edge, bypass + 1, edge_len);
    add_ver(graph, *(edge->ver_lst));
    if ((*graph)->len < vert_len && cicled == 0)
    {
        for (int i = 0; i < edge_len; i++)
            if (((*((*edges) + i))->visited) == 0)
                top_sort(graph, edges, *((*edges) + i), bypass + 1, vert_len, edge_len);
    }
    else if (cicled == 1)
        printf("impossible to sort");
    else
        print_graph(*graph);
}


//input processing
void prc(FILE * file, int verts_len, int edges_len){
    int str_len = 0;
    int edge_start;
    int edge_end;
    int test = 0;

    Edge **edges = (Edge **) malloc(sizeof (Edge) * edges_len);
    for (int i = 0; (i < edges_len) && (test == 0); i++) {
        if (fscanf(file, "%d %d", &edge_start, &edge_end) != 2)
        {
            fclose(file);
            puts("bad number of lines");
            test = 1;
        }
        else if (!((edge_start > 0) && (edge_start <= verts_len)&&(edge_end > 0)&&
        (edge_end <= verts_len))){
            printf("bad vertex");
            test = 1;
        }
        else if (edge_start != edge_end){
            int *edge_ends = (int *) malloc(sizeof(int) * 2);
            *edge_ends = edge_start;
            *(edge_ends + 1) = edge_end;

            *(edges + str_len++) = create_edge(edge_ends);
        }
    }

    if (test == 0) {
        Graph *graph = create_graph(*(*edges)->ver_lst);
        top_sort(&graph, &edges, *edges, 1, verts_len, edges_len);
    }
}


int main()
{
    FILE *file = fopen("in.txt", "r");
    int verts_len;
    int edges_len;

    fscanf(file, "%d", &verts_len);
    fscanf(file, "%d", &edges_len);

    if (size_chk(verts_len, edges_len) == 0)
        prc(file, verts_len, edges_len);

    return 0;
}