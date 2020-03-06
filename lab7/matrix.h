#ifndef MATRIX_H
#define MATRIX_H

#include "structs.h"

int create_matrix(matrix_t *graph);
void delete_matrix(matrix_t *graph);
int fill_graph(matrix_t * graph);
void show_matr_gv(const matrix_t *a);
void find_path(int v, matrix_t *g, int *used);
int has_path(int s, int f, matrix_t *g, int *used_for_p);
void dfs (int v, int parent, int cur_distance, int s, int f, matrix_t *g,
          matrix_t *distance, int *used, matrix_t *graph_copy, int *used_for_p);
void show_result(matrix_t *distance);

#endif
