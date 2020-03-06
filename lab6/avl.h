#ifndef AVL_H
#define AVL_H
#include "structs.h"

avl_tree_node* right_rotate(avl_tree_node *node);
avl_tree_node* left_rotate(avl_tree_node *node);
int balance_insert(avl_tree_node **root);
int balance_remove(avl_tree_node **root, int res);
int get_min(avl_tree_node **root, avl_tree_node **res);

avl_tree_node* create_avl(int value);
int avl_insert(avl_tree_node **root, int value, int *temp_cmp);
int avl_remove(avl_tree_node **root, int value, int *temp_cmp);
avl_tree_node* avl_find(avl_tree_node *node, const int value, int *comp_num);
void delete_tree_avl(avl_tree_node **node);

void avl_in_order(avl_tree_node *node, int deep);
int _print_t_avl(avl_tree_node *tree, int is_left, int offset, int depth, char s[20][255]);
void print_t_avl(avl_tree_node *tree);

#endif // AVL_H
