#include "matrix.h"

int create_matrix(matrix_t *graph)
{
    if (graph->size <= 0)
    {
        printf("Not correxct size of graph\n");
        return ERROR_CODE;
    }

    graph->data = (int**)malloc(sizeof(int*) * graph->size);

    if (graph->data == NULL)
    {
        printf("Malloc error");
        return ERROR_CODE;
    }
    int *temp_arr;
    for (int i = 0; i < graph->size; i++)
    {
        temp_arr = (int*)malloc(sizeof(int) * graph->size);
        if (temp_arr == NULL)
        {
            printf("Malloc error\n");
            for (int j = 0; j <= i; j++)
                free(graph->data[j]);
            return ERROR_CODE;
        }
        graph->data[i] = temp_arr;
    }
    temp_arr = NULL;

    for (int i = 0; i < graph->size; i++)
        for (int j = 0; j < graph->size; j++)
            graph->data[i][j] = 0;
    return 1;
}

void delete_matrix(matrix_t *graph)
{
    for (int i = 0; i < graph->size; i++)
    {
        free(graph->data[i]);
    }
    free(graph->data);
}


int fill_graph(matrix_t * graph)
{
    int rc;
    FILE *f = fopen("input.txt", "r");
    if (f == NULL)
    {
        printf("Error in file");
        return ERROR_CODE;
    }
    rc = 1;
    int n;
    int to, from, len;
    rc = fscanf(f, "%d", &n);
    if (rc == -1)
    {
        printf("Empty file");
        return ERROR_CODE;
    }
    graph->size = n;
    if (create_matrix(graph) == ERROR_CODE)
        return ERROR_CODE;

    while (!feof(f))
    {
        rc = fscanf(f, "%d %d %d", &to, &from, &len);
        if (rc == -1 && !feof(f))
            return TRUE;
        else if (rc == -1)
            break;

        if (to >= n || from >= n)
        {
            printf("Graph dont have this vertex\n");
            return ERROR_CODE;
        }
//        if (len <= 0)
//        {
//            printf("Way can not be negative\n");
//            return ERROR_CODE;
//        }

        if (to != from)
        {
            graph->data[to][from] = len;
            graph->data[from][to] = len;
        }
    }
    for (int i = 0; i < graph->size; i++)
    {
        for (int j = 0; j < graph->size; j++)
            printf("%d ", graph->data[i][j]);
        printf("\n");
    }
    fclose(f);
    return TRUE;
}

void show_matr_gv(const matrix_t *a)
{
    if (a == NULL)
    {
        return;
    }
    int *used = malloc(sizeof(int) * a->size);
    if (used == NULL)
    {
        printf("Malloc error");
        return;
    }
    for (int i = 0; i < a->size; i++)
    {
        used[i] = 0;
    }

    FILE *f = fopen("output.gv", "w");
    fprintf(f, "graph My_graph {\n");
    for (int i = 0; i < a->size; i++)
    {
        for (int j = i; j < a->size; j++)
        {
            if (a->data[i][j] != 0)
            {
                fprintf(f, "%d--%d[label=\"%d \"];\n", i, j, a->data[i][j]);
                used[i] = 1;
                used[j] = 1;
            }
        }
    }
    for (int i = 0; i < a->size; i++)
    {
        if (used[i] == 0)
            fprintf(f, "%d--%d;\n", i, i);
    }
    fprintf(f, "}\n");
    fclose(f);
    free(used);
    system("output.gv");
}

void find_path(int v, matrix_t *g, int *used)
{
    if (used[v])
        return;
    used[v] = 1;
    for (int i = 0; i < g->size; ++i)
        if (!used[i] && g->data[v][i] > 0)
            find_path(i, g, used);
}

int has_path(int s, int f, matrix_t *g, int *used_for_p)
{
    for (int k = 0; k < g->size; k++)
        used_for_p[k] = 0;

    find_path(s, g, used_for_p);
    int answer = used_for_p[f];
    return answer;
}

void dfs (int v, int parent, int cur_distance, int s, int f, matrix_t *g,
          matrix_t *distance, int *used, matrix_t *graph_copy, int *used_for_p)
{
    if (v == f)
    {
        distance->data[s][f] = (distance->data[s][f] > cur_distance) ? distance->data[s][f] : cur_distance;
        return;
    }

    if (used[v])
    {
        if (has_path(s, f, g, used_for_p))
            distance->data[s][f] = INF;
        return;
    }

    used[v] = TRUE;
    for (int i = 0; i < g->size; i++)
    {
        if (distance->data[s][f] != INF)
        {
            if (g->data[v][i] != 0 && i != parent)
            {
                if (v == s)
                {
                    for (int j = 0; j < g->size; j++)
                    {
                        if (i != j)
                        {
                            g->data[v][j] = 0;
                            g->data[j][v] = 0;
                        }
                    }

                    for (int k = 0; k < g->size; k++)
                        used[k] = 0;
                }
                dfs(i, v, cur_distance + g->data[i][v], s, f, g, distance, used, graph_copy, used_for_p);
                if (v == s)
                {
                    for (int k = 0; k < g->size; k++)
                        for (int j = 0; j < g->size; j++)
                            g->data[k][j] = graph_copy->data[k][j];
                }
            }
        }
    }
}

void show_result(matrix_t *distance)
{
    printf("Max ways\n");
    for (int i = 0; i < distance->size; i++)
    {
        for (int j = i + 1; j < distance->size; j++)
        {
            printf("max way from %d to %d ", i, j);
            int to_print;
            to_print = (distance->data[i][j] > distance->data[j][i]) ? distance->data[i][j] : distance->data[j][i];
            if (to_print == INF)
                printf("= INF\n");
            else if (to_print == 0)
                printf("does not exist\n");
            else
                printf("= %d\n", to_print);
        }
        printf("\n");
    }
}
