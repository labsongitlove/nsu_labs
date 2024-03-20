#include "stdio.h"
#include "stdlib.h"

#include "limits.h"

#include "graph.c"


//size checker
int size_chk(int key, int edge, int start, int end)
{
    int test = 0;

    if (key == 0){
        printf("bad vertex");
        test = 1;
    }
    else if (key < 0 || key > 5000)
    {
        puts("bad number of vertices");
        test = 1;
    }
    else if (edge < 0 || edge > key * (key - 1) / 2)
    {
        puts("bad number of edges");
        test = 1;
    }
    else if (start < 1 || start > key || end < 1 || end > key)
    {
        puts("bad key");
        test = 1;
    }
    return test;
}


//compare distance to vortex now and new distance
int min(Graph* que, Dijstp* steps)
{
    int min_value = -1;
    long int min_dist = LONG_MAX;
    if (que){
        while (que != NULL)
        {
            int key = que->key;
            if (!(steps + key)->done && (steps + key)->dist <= min_dist)
            {
                min_dist = (steps + key)->dist;
                min_value = key;
            }
            que = que->next;
        }
        return min_value;
    }
    return -1;
}


//main algorithm
void dijkstra(Adj* adj, int start, int end)
{
    int path_len = 0;
    int min_edge = 0;
    int adj_key;
    int* path = (int*)malloc(sizeof(int) * adj->key);

    Dijstp* steps = (Dijstp*)malloc(adj->key * sizeof(Dijstp));
    Graph* queue = NULL;
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    Graph* temp;

    //first step: set to all adjacent max distance
    for (int i = 0; i < adj->key; i++)
    {
        (steps + i)->dist = LONG_MAX;
        (steps + i)->done = 0;
        (steps + i)->key = -1;
    }
    (steps + start - 1)->dist = 0;

    graph->key = start - 1;
    graph->value = 0;
    graph->next = NULL;
    queue = graph;

    //next steps: until we've gone through all the vortex
    while ((queue != NULL) && (min_edge != -1))
    {
        min_edge = min(queue, steps);
        if (min_edge != -1) {
            (steps + min_edge)->done = 1;
            temp = *(adj->adj_lst + min_edge);

            while (temp != NULL)
            {
                adj_key = temp->key - 1;
                long int new_path = (steps + min_edge)->dist + temp->value;

                if (new_path <= (steps + adj_key)->dist)
                {
                    (steps + adj_key)->dist = new_path;
                    (steps + adj_key)->key = min_edge;

                    graph = (Graph*)malloc(sizeof(Graph));
                    graph->key = adj_key;
                    graph->value = new_path;
                    graph->next = queue;

                    queue = graph;
                }
                temp = temp->next;
            }
        }
    }


    int cmbck = end - 1;
    while (cmbck != -1)
    {
        *(path + path_len++) = cmbck + 1;
        cmbck = (steps + cmbck)->key;
    }

    for (int i = 0; i < adj->key; i++)
    {
        if ((steps + i)->dist == LONG_MAX)
            printf("oo ");
        else if ((steps + i)->dist > INT_MAX)
            printf("INT_MAX+ ");
        else
            printf("%ld ", (steps + i)->dist - (steps + start - 1)->dist);
    }
    printf("\n");
    int pthcnt = 0;
    int lencnt = 0;

    for (int i = 0; i < path_len; i++)
    {
        if (*(path + i) == start || *(path + i) == end)
            pthcnt++;
        if ((steps + i + 1)->dist >= INT_MAX &&
            (steps + i + 1)->dist != LONG_MAX)
            lencnt++; //test on len of distance
    }

    int test = 0;

    if ((steps + path_len - 1)->dist > INT_MAX && lencnt > 2)
    {
        puts("overflow");
        test = 1;
    }
    else if (pthcnt != 2 && start != end)
    {
        puts("no path");
        test = 1;
    }

    for (int i = 0; (i < path_len) && (test == 0); i++)
        printf("%d ", *(path + i));
    printf("\n");

    free(steps);
    free(graph);
}


//input processing
void prc(int key, int start, int end, int edge){
    int str_len = 0;
    int edge_start;
    int edge_end;
    int test = 0;
    long int value;

    Adj *adj = create_adj(key);
    for (int i = 0; (i < edge) && (test == 0); i++) {
        scanf("%d %d %ld", &edge_start, &edge_end, &value);
        if (value < 0 || value > INT_MAX) {
            puts("bad length");
            test = 1;
        }
        else{
            int *edge_ends = (int *) malloc(sizeof(int) * 2);
            *edge_ends = edge_start;
            *(edge_ends + 1) = edge_end;

            add_edge(adj, edge_ends, value);
            str_len++;
        }
    }

    if (test == 0){
        if ((str_len < edge))
            puts("bad number of lines");
        else dijkstra(adj, start, end);
    }

    free(adj);
}


int main()
{
    int key;
    int start;
    int end;
    int edge;

    scanf("%d", &key);
    scanf("%d %d", &start, &end);
    scanf("%d", &edge);

    if (size_chk(key, edge, start, end) == 0)
        prc(key, start, end, edge);

    return 0;
}