#include "stdio.h"
#include "stdlib.h"

#include "graph.c"


//quick sort
Edge ** srt(Edge **edges, int left, int right){
    int i = left;
    int j = right;

    int pvt = (left + right) / 2 + 1;

    if (left < right) {
        while (i <= j) {
            while ((*(edges + i))->value < (*(edges + pvt))->value) i++;
            while ((*(edges + j))->value > (*(edges + pvt))->value) j--;

            if (i <= j) {
                //printf("%d %d %d %d&", *(*(edges + i))->ver_lst, *(*(edges + j))->ver_lst, *((*(edges + i))->ver_lst + 1), *((*(edges + j))->ver_lst + 1));
                if ((*(edges + i))->value != (*(edges + pvt))->value ||
                    *(*(edges + i))->ver_lst > *(*(edges + j))->ver_lst ||
                    *((*(edges + i))->ver_lst + 1) > *((*(edges + j))->ver_lst + 1)) {
                    //printf("%d %d", i, j);
                    Edge *vr = *(edges + i);
                    *(edges + i++) = *(edges + j);
                    *(edges + j--) = vr;
                } else{
                    i++; j--;
                }
            }
        }
        srt(edges, left, j);
        srt(edges, i, right);
    }
    return edges;
}


//making first num > second
int * srt_int(int *nums){
    if (*nums > *(nums + 1)){
        int temp = *nums;
        *nums = *(nums + 1);
        *(nums + 1) = temp;
    }
    return nums;
}


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


//count how many vertex of edge in cluster
Edge * edge_cnt(Edge ** edges, Clust *clust, int edges_len,
               int *ver) {
    Edge *edge = NULL;
    int vers_len = 0;

    for (int i = 0; i < edges_len && vers_len != 1; i++) {
        vers_len = 0;
        for (int k = 0; k < clust->len && vers_len < 2; k++) {
            if (*((*(edges + i))->ver_lst) == *(clust->ver_lst + k)) {
                *ver = *((*(edges + i))->ver_lst + 1);
                vers_len++;
                edge = *(edges + i);
            }
            if (*((*(edges + i))->ver_lst + 1) == *(clust->ver_lst + k)) {
                *ver = *((*(edges + i))->ver_lst);
                vers_len++;
                edge = *(edges + i);
            }
        }
        if (vers_len == 2){
            *ver = -1;
            edge = NULL;
        }
    }

    return edge;
}


//main algoritm
Clust * prim (Edge **edges, int edges_len){
    Clust *clust;
    clust = create_clust((*edges)->ver_lst);
    int ver = -1;
    Edge * edge_add;

    if (edges_len > 1)
        edge_add = edge_cnt(edges, clust, edges_len, &ver);
    else
        clust->len = -1;

    while (ver != -1) {
        add_ver(clust, ver, edge_add->ver_lst);
        ver = -1;
        edge_add = edge_cnt(edges, clust, edges_len, &ver);
    }
    return clust;
}


//input processing
void prc(FILE * file, int verts_len, int edges_len){
    int str_len = 0;
    int edge_start;
    int edge_end;
    int test = 0;
    long long int value;

    Edge **edges = (Edge **) malloc(sizeof (Edge) * edges_len);
    for (int i = 0; (i < edges_len) && (test == 0); i++) {
        if (fscanf(file, "%d %d %lld", &edge_start, &edge_end, &value) != 3)
        {
            fclose(file);
            puts("bad number of lines");
            test = 1;
        }
        else if (value < 0 || value > INT_MAX) {
            printf("bad length");
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
            edge_ends = srt_int(edge_ends);

            *(edges + str_len++) = create_edge(value, edge_ends);
        }
    }

    if (test == 0) {
        srt(edges, 0, edges_len - 1);
        Clust *clust = prim(edges, str_len);
        if ((clust->len != verts_len || edges_len == 0) && verts_len != 1)
            printf("no spanning tree");
        else
            for (int i = 0; i < clust->len * 2 - 2; i += 2) {
                int *nums = srt_int(clust->edg_lst + i);
                printf("%d %d\n", *nums, *(nums + 1));
                }
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