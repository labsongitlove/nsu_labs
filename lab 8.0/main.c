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
                if ((*(edges + i))->value != (*(edges + pvt))->value ||
                    *(*(edges + i))->ver_lst > *(*(edges + j))->ver_lst ||
                    *((*(edges + i))->ver_lst + 1) > *((*(edges + j))->ver_lst + 1)) {
                    Edge *vr = *(edges + i);
                    *(edges + i++) = *(edges + j);
                    *(edges + j--) = vr;
                } else
                    i++; j--;

            }
        }
        srt(edges, left, j);
        srt(edges, i, right);
    }
    return edges;
}


//making first number > second
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


//merge two clusters
Clust * merge(Clust *clust1, Clust *clust2, int mrg_elem){
    int cnt = 0;
    for (int i = 0; i < clust2->len; i++)
        if (*(clust2->ver_lst + i) != mrg_elem){
            *(clust1->ver_lst + clust1->len) = *(clust2->ver_lst + i);
            *(clust1->edg_lst + clust1->len * 2 - 2) = *(clust2->edg_lst + cnt * 2);
            *(clust1->edg_lst + clust1->len++ * 2 - 1) =
                    *(clust2->edg_lst + cnt++ * 2 + 1);
        }
    return clust1;
}


//count vertex
int * vers_cnt(Edge *edge, Clust **clust_lst, int clust_len){
    int vers_len = 0;
    int ver = 0;
    int clust_ind = 0;

    for (int cnt = 0; cnt < clust_len; cnt++)
        for (int i = 0; i < (*(clust_lst + cnt))->len; i++){
            int clust_ver = *((*(clust_lst + cnt))->ver_lst + i);
            if (clust_ver == *(edge->ver_lst)){
                if (clust_ind != cnt && ver == *(edge->ver_lst) &&
                vers_len == 1)
                    merge(*(clust_lst + clust_ind), *(clust_lst + cnt),
                          clust_ver);
                else{
                    vers_len++;
                    ver = *((edge->ver_lst) + 1);
                    clust_ind = cnt;
                }
            }
            if (clust_ver == *((edge->ver_lst) + 1)) {
                if (clust_ind != cnt && ver == *((edge->ver_lst) + 1) &&
                    vers_len == 1)
                    merge(*(clust_lst + clust_ind), *(clust_lst + cnt),
                          clust_ver--);
                else{
                    vers_len++;
                    ver = *(edge->ver_lst);
                    clust_ind = cnt;
                }
            }
        }

    int *res = (int*)malloc(sizeof(int) * 2);
    *res = ver;
    *(res + 1) = clust_ind;

    if (vers_len > 1)
        *res = -1;
    return res;
}


//main algoritm
Clust * kraskal(Edge **edges, int edges_len){
    srt(edges, 0, edges_len - 1);
    Clust **clust_lst = (Clust **)malloc(sizeof (Clust*));
    int clust_len = 0;
    int *ver_in;

    for (int i = 0; i < edges_len; i++){
        ver_in = vers_cnt(*(edges + i), clust_lst, clust_len);
        if (*ver_in == 0){
            clust_lst = (Clust **) realloc(clust_lst,
                                           ++clust_len * sizeof(Clust *));
            (*(clust_lst + clust_len - 1)) = create_clust((*(edges + i))->ver_lst);

        }
        else if (*ver_in > 0)
            (*(clust_lst + clust_len - 1)) = add_ver(*(clust_lst +
                    *(ver_in + 1)), *ver_in, (*(edges + i))->ver_lst);
    }

    return *(clust_lst + 0);
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
        Clust *clust = kraskal(edges, str_len);
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