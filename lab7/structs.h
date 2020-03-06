#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <stdlib.h>

#define ERROR_CODE -1
#define TRUE 1
#define FALSE 0
#define INF 100000

typedef struct
{
    int size;
    int **data;
}matrix_t;

#endif
