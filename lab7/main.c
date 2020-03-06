#include "structs.h"
#include "matrix.h"

unsigned long long tick(void)
{
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc" : "=A" (d) );

    return d;
}

int main()
{
    matrix_t graph, g_copy;
    matrix_t distance;
    graph.size = 0;
    fill_graph(&graph);
    distance.size = graph.size;
    create_matrix(&distance);

    g_copy.size = graph.size;
    create_matrix(&g_copy);
    for (int i = 0; i < graph.size; i++)
        for (int j = 0; j < graph.size; j++)
            g_copy.data[i][j] = graph.data[i][j];

    int *visited = malloc(sizeof(int) * graph.size);
    int *d = malloc(sizeof(int) * graph.size);
    int *p = malloc(sizeof(int) * graph.size);
    int *used_for_p = malloc(sizeof(int) * graph.size);
    if (p == NULL || visited == NULL || d == NULL || used_for_p == NULL)
    {
        printf("Malloc error");
        return ERROR_CODE;
    }
    int start, stop, time_sum = 0;

    show_matr_gv(&graph);
    for (int i = 0; i < graph.size; i++)
    {
        for (int j = 0; j < graph.size; j++)
        {
            if (i != j)
            {
                for (int k = 0; k < graph.size; k++)
                    visited[k] = 0;
                start = tick();
                dfs(i, -1, 0, i, j, &graph, &distance, visited, &g_copy, used_for_p);
                stop = tick();
                time_sum += stop - start;
            }
        }
    }
    show_result(&distance);
    free(visited);
    free(d);
    free(used_for_p);
    free(p);
    delete_matrix(&graph);
    delete_matrix(&distance);
    printf("\nTIME = %d tick\n", time_sum);
    printf("MEMORY %ld byte\n\n", sizeof(int) * graph.size * graph.size + sizeof(int));
}

