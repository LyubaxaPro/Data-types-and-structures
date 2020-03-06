#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "avl.h"
extern int errno;

avl_tree_node* right_rotate(avl_tree_node *node)
{
    static const int array[6][4] = { \
        {-1,-1,+1,+1}, \
        {-1,+0,+1,+0}, \
        {-1,+1,+2,+0}, \
        {-2,-1,+0,+0}, \
        {-2,-2,+0,+1}, \
        {-2,+0,+1,-1}  \
        }; \
    avl_tree_node *n_left_ptr = node->left;
    avl_tree_node *n_left_right_ptr = n_left_ptr->right;

    n_left_ptr->right = node;
    node->left = n_left_right_ptr;

    for(int n=0;n<6;n++) if(array[n][0] == node->balance && array[n][1] == n_left_ptr->balance) {
        n_left_ptr->balance = array[n][2];
        node->balance = array[n][3];
        break;
    }
    return n_left_ptr;
}

avl_tree_node* left_rotate(avl_tree_node *node)
{
    static const int array[6][4]={ \
        {+1,-1,-2,+0}, \
        {+1,+0,-1,+0}, \
        {+1,+1,-1,-1}, \
        {+2,+0,-1,+1}, \
        {+2,+1,+0,+0}, \
        {+2,+2,+0,-1}
    }; \

    avl_tree_node *n_right_ptr = node->right;
    if (node->right == NULL)
        return n_right_ptr;
    avl_tree_node *n_right_left_ptr = n_right_ptr->left;
    n_right_ptr->left = node;
    node->right = n_right_left_ptr;

    for(int n=0;n<6;n++) if(array[n][0] == node->balance && array[n][1] == n_right_ptr->balance) {
        n_right_ptr->balance = array[n][2];
        node->balance = array[n][3];
        break;
    }
    return n_right_ptr;
}

int balance_insert(avl_tree_node **root)
{
    avl_tree_node *node = *root;
    if (node ->balance > 1){
        if (node -> right->balance < 0) node ->right = right_rotate(node->right);
        *root = left_rotate(node);
        return TRUE;
    }
    if (node->balance< -1)
    {
        if (node->left->balance > 0) node ->left = left_rotate(node->left);
        *root = right_rotate(node);
        return TRUE;
    }
    return FALSE;
}

avl_tree_node* create_avl(int value)
{
    avl_tree_node* new = NULL;

    new = malloc(sizeof(avl_tree_node));
    if (new == NULL)
    {
        return NULL;
    }

    new->left = NULL;
    new->right = NULL;
    new->data = value;
    new->balance = 0;

    return new;
}

int avl_insert(avl_tree_node **root, int value, int *temp_cmp)
{
    *temp_cmp += 1;
    int res = FALSE;
    avl_tree_node* node = *root;
    if (!node)
    {
        *root = create_avl(value);
        return TRUE;
    }
    if (value == node->data)
        return FALSE;
    if (value<node ->data)
        res = avl_insert(&node->left,  value, temp_cmp) && --(node->balance);
    else
        res = avl_insert(&node->right, value, temp_cmp) && ++(node->balance);
    if (balance_insert(root))
        res = FALSE;
    return res;
}

int balance_remove(avl_tree_node **root, int res)
{
    avl_tree_node *node = *root;
    if (node->balance > 1)
    {
        res = !!node->right->balance;
        if (node->right != NULL && node->right->balance < 0)
            node->right = right_rotate(node->right);
        *root = left_rotate(node);
    }
    if (node->balance < -1) {
        res = node->left->balance;
        if (node->left->balance > 0)
            node -> left = left_rotate(node->left);
        *root = right_rotate(node);
    }
    return res;
}

int get_min(avl_tree_node **root, avl_tree_node **res)
{
    avl_tree_node *node = *root;
    if (node->left)
    {
        if (get_min(&node->left, res) && !++node->balance)
            return TRUE;
        return balance_remove(root, FALSE);
    }
    *res = node;
    *root = node->right;
    return TRUE;
}

int avl_remove(avl_tree_node **root, int value, int *temp_cmp)
{
    *temp_cmp += 1;
    int rc = FALSE;
    avl_tree_node *node = *root;
    if (!node)
    {
        printf("\nTree is empty!");
        return rc;
    }
    if (node->data < value)
    {
        if (avl_remove(&node->right, value, temp_cmp) && !--node->balance) rc = TRUE;
    }
    else if (node->data>value)
    {
        if (avl_remove(&node->left, value, temp_cmp) && !++node->balance) rc = TRUE;
    }
    else
    {
        if (!node->right)
        {
            *root = node->left;
            free(node);
            return TRUE;
        }
        rc = get_min(&node->right, root);
        if (*root != NULL)
        {
            (*root)->balance = node->balance;
            (*root)->left = node->left;
            (*root)->right = node->right;
        }
        free(node);
        if (rc) rc = !--(*root)->balance;
    }
    return balance_remove(root, rc);
}

void avl_in_order(avl_tree_node *node, int deep)  // левое поддерево, узел, правое поддерево
{
    if (node == NULL)
        return;
    avl_in_order(node->left, deep + 1);
    printf("(val = %d, balance = %d, deep = %d)\n", node->data, node->balance, deep);
    avl_in_order(node->right, deep + 1);
}

avl_tree_node* avl_find(avl_tree_node *node, const int value, int *comp_num)
{
    *comp_num += 1;
    if (node == NULL)
    {
        return NULL;
    }

    if (node->data == value)
    {
        return node;
    }

    if (node->data > value)
    {
        return avl_find(node->left, value, comp_num);
    }

    if (node->data < value)
    {
        return avl_find(node->right, value, comp_num);
    }
    return NULL;
}



void delete_tree_avl(avl_tree_node **node)  // post_order
{
    if (*node == NULL)
        return;
    delete_tree_avl(&(*node)->left);
    delete_tree_avl(&(*node)->right);
    free(*node);
    *node = NULL;
}


int _print_t_avl(avl_tree_node *tree, int is_left, int offset, int depth, char s[20][255])
{
    char b[20];
    int width = 5;

    if (!tree) return 0;

    sprintf(b, "(%03d)", tree->data);

    int left  = _print_t_avl(tree->left,  1, offset,                depth + 1, s);
    int right = _print_t_avl(tree->right, 0, offset + left + width, depth + 1, s);

#define COMPACT


#ifdef COMPACT
    for (int i = 0; i < width; i++)
        s[depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (int i = 0; i < width + right; i++)
            s[depth - 1][offset + left + width/2 + i] = '-';

        s[depth - 1][offset + left + width/2] = '.';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[depth - 1][offset - width/2 + i] = '-';

        s[depth - 1][offset + left + width/2] = '.';
    }
#else
    for (int i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (int i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset + left + width + right + width/2] = '+';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset - width/2 - 1] = '+';
    }
#endif

    return left + width + right;
}

void print_t_avl(avl_tree_node *tree)
{
    char s[20][255];
    for (int i = 0; i < 20; i++)
        sprintf(s[i], "%80s", " ");

    _print_t_avl(tree, 0, 0, 0, s);

    for (int i = 0; i < 20; i++)
        printf("%s\n", s[i]);
}
