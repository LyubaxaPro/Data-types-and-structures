#ifndef DDP_H
#define DDP_H

#include "structs.h"

ddp_tree_node* create_ddp(int value);
void ddp_add(ddp_tree_node **node, const int value, int *comp_num);
ddp_tree_node* ddp_find(ddp_tree_node *node, const int value, int *comp_num);
void ddp_delete_node(ddp_tree_node **node);
int ddp_remove(ddp_tree_node **node, const int value, int *comp_num);
int avl_insert(avl_tree_node **root, int value, int *temp_cmp);
void delete_tree_ddp(ddp_tree_node **node);

void make_avl_from_ddp(ddp_tree_node *node_ddp, avl_tree_node **root_avl, int*sum);

int _print_t_ddp(ddp_tree_node *tree, int is_left, int offset, int depth, char s[20][255]);
void print_t_ddp(ddp_tree_node *tree);
void ddp_in_order(ddp_tree_node *node, int deep);

#endif // DDP_H
