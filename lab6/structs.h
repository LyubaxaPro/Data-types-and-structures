#ifndef STRUCTS_H
#define STRUCTS_H

#define SUCCESS 0
#define MALLOC_ERROR 2
#define TRUE 1
#define CORRECT_ERROR 3
#define FALSE 0
#define FILE_ERROR 5


struct ddp_node
{
    int data;
    struct ddp_node  *left;
    struct ddp_node  *right;
};

struct avl_node
{
    int data;
    struct avl_node  *left;
    struct avl_node  *right;
    int balance; // -1 left is bigger, 0 the same, 1 right is bigger
};

typedef struct
{
    int data;
    int is_free;
    int is_deleted;
}data_record;

typedef struct
{
    data_record *arr;
    int table_size;
    int cur_size;
}hash_table;

typedef struct ddp_node ddp_tree_node;

typedef struct avl_node avl_tree_node;

#endif // STRUCTS_H
