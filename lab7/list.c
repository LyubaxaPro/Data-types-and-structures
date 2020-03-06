#include "structs.h"
#include "list.h"

struct list_node
{
    int data;
    int to;
    struct list_node *p_next;
};

typedef struct
{
    int size;
    struct list_node** arr;
}list_t;

int create_list(list_t *graph)
{
    if (graph->size <= 0)
    {
        printf("Not correxct size of graph\n");
        return ERROR_CODE;
    }

    graph->arr = (struct list_node**)malloc(sizeof(struct list_node*) * graph->size);

    if (graph->arr == NULL)
    {
        printf("Malloc error");
        return ERROR_CODE;
    }
    for (int i = 0; i < graph->size; i++)
    {
        graph->arr[i] = NULL;
    }
    return TRUE;
}

void delete_list(list_t *graph)
{
    struct list_node *temp;
    struct list_node *t_next;

    for (int i = 0; i < graph->size; i++)
    {
        t_next = graph->arr[i];
        while (t_next != NULL)
        {
            temp = t_next;
            t_next = t_next->p_next;
            free(temp);
        }
    }
    free(graph->arr);
}

int fill_g_list(list_t *graph)
{
    FILE *f = fopen("input.txt", "r");
    if (f == NULL)
    {
        printf("Error in file");
        return ERROR_CODE;
    }
    int n;
    int to, from, len;
    fscanf(f, "%d", &n);
    graph->size = n;
    if (create_list(graph) == ERROR_CODE)
        return ERROR_CODE;

    while (!feof(f))
    {
        fscanf(f, "%d %d %d", &to, &from, &len);

        if (to >= n || from >= n)
        {
            printf("Graph dont have this vertex\n");
            return ERROR_CODE;
        }
        if (len <= 0)
        {
            printf("Way can not be negative\n");
            return ERROR_CODE;
        }

        if (to != from)
        {
            struct list_node *temp_ptr = (struct list_node*)malloc(sizeof(struct list_node));
            if (temp_ptr == NULL)
            {
                printf("Malloc error");
                return ERROR_CODE;
            }

            temp_ptr->data = len;

            struct list_node *temp_p = (struct list_node*)malloc(sizeof(struct list_node));
            if (temp_p == NULL)
            {
                printf("Malloc error");
                return ERROR_CODE;
            }

            temp_p->data = len;

            struct list_node *temp;
            temp = graph->arr[from];

            temp_ptr->to = to;
            temp_p->to = from;

            if (temp == NULL)
            {
                graph->arr[from] = temp_ptr;
            }
            else
            {
                while(temp->p_next != NULL)
                    temp = temp->p_next;
                temp->p_next = temp_ptr;
            }

            temp = graph->arr[to];

            if (temp == NULL)
            {
                graph->arr[to] = temp_p;
            }
            else
            {
                while(temp->p_next != NULL)
                    temp = temp->p_next;
                temp->p_next = temp_p;
            }
        }
    }
    fclose(f);
    return TRUE;
}
