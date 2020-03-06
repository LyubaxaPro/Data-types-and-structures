#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "ddp.h"
extern int errno;

ddp_tree_node* create_ddp(int value)
{
    ddp_tree_node *new = NULL;

    if ((new = malloc(sizeof(ddp_tree_node))) == NULL)
    {
        return NULL;
    }

    new->left = NULL;
    new->right = NULL;
    new->data = value;

    return new;
}

void ddp_add(ddp_tree_node **node, const int value, int *comp_num)
{
    *comp_num += 1;

    if (*node == NULL)
    {
        ddp_tree_node *temp = create_ddp(value);
        if (temp != NULL)
            *node = temp;
        return;
    }
    if ((*node)->data > value)
    {
        ddp_add(&((*node)->left), value, comp_num);
    }
    if ((*node)->data < value)
    {
        ddp_add(&((*node)->right), value, comp_num);
    }
}

ddp_tree_node* ddp_find(ddp_tree_node *node, const int value, int *comp_num)
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
        return ddp_find(node->left, value, comp_num);
    }

    if (node->data < value)
    {
        return ddp_find(node->right, value, comp_num);
    }
    return NULL;
}

void ddp_delete_node(ddp_tree_node **node)
{
    if ((*node)->left == NULL)
    {
        ddp_tree_node *right = (*node)->right;
        free(*node);
        *node = right;
    }
    else if ((*node)->right == NULL)
    {
        ddp_tree_node *left = (*node)->left;
        free(*node);
        *node = left;
    }
    else
    {
        ddp_tree_node *min_parent = *node;
        ddp_tree_node *min_node = (*node)->right;
        while(min_node->left != NULL)
        {
            min_parent = min_node;
            min_node = min_node->left;
        }
        (*node) ->data = min_node->data;
        if (min_parent->left == min_node)
            min_parent->left = min_node->right;
        else
            min_parent->right = min_node->right;
        free(min_node);
    }
}

int ddp_remove(ddp_tree_node **node, const int value, int *comp_num)
{
    {
        *comp_num += 1;
        if (node == NULL)
        {
            printf("\nTree is empty!\n");
            return FALSE;
        }

        if ((*node)->data == value)
        {
            ddp_delete_node(node);
            return TRUE;
        }

        if ((*node)->data > value)
        {
            if ((*node)->left == NULL)
            {
                printf("\nDont have this key in tree!\n");
                return FALSE;
            }
            ddp_remove(&(*node)->left, value, comp_num);
            return TRUE;
        }

        if ((*node)->data < value)
        {
            if ((*node)->right == NULL)
            {
                printf("\nDont have this key in tree!\n");
                return FALSE;
            }
            ddp_remove(&(*node)->right, value, comp_num);
            return TRUE;
        }
    }
    return FALSE;
}

void ddp_in_order(ddp_tree_node *node, int deep)  // левое поддерево, узел, правое поддерево
{
    if (node == NULL)
        return;
    ddp_in_order(node->left, deep + 1);
    printf("(val = %d, deep = %d)\n", node->data, deep);
    ddp_in_order(node->right, deep + 1);
}

void delete_tree_ddp(ddp_tree_node **node)  // post_order
{
    if (*node == NULL)
        return;
    delete_tree_ddp(&(*node)->left);
    delete_tree_ddp(&(*node)->right);
    free(*node);
    *node = NULL;
}

void make_avl_from_ddp(ddp_tree_node *node_ddp, avl_tree_node **root_avl, int *sum)
{
    int temp_cmp = 0;
    if (node_ddp == NULL)
        return;
    avl_insert(root_avl, node_ddp->data, &temp_cmp);
    *sum += temp_cmp;
    make_avl_from_ddp(node_ddp->left, root_avl, sum);
    make_avl_from_ddp(node_ddp->right, root_avl, sum);
}

int _print_t_ddp(ddp_tree_node *tree, int is_left, int offset, int depth, char s[20][255])
{
    char b[20];
    int width = 5;

    if (!tree) return 0;

    sprintf(b, "(%03d)", tree->data);

    int left  = _print_t_ddp(tree->left,  1, offset,                depth + 1, s);
    int right = _print_t_ddp(tree->right, 0, offset + left + width, depth + 1, s);

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

void print_t_ddp(ddp_tree_node *tree)
{
    char s[20][255];
    for (int i = 0; i < 20; i++)
        sprintf(s[i], "%80s", " ");

    _print_t_ddp(tree, 0, 0, 0, s);

    for (int i = 0; i < 20; i++)
        printf("%s\n", s[i]);
}
