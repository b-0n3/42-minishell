//
// Created by Abdelouahad Ait hamd on 11/10/21.
//

#include "minishell.h"

t_node  *to_node(t_token *this)
{
    return NULL;
}

t_node  *new_node()
{
    t_node  *new;

    new  = malloc(sizeof(t_token));
    if (new == NULL)
        return (NULL);
    new_array_list(&new->args, 2, sizeof(t_token));
    new_array_list(&new->right_nodes, 2, sizeof(t_node));
    new_array_list(&new->left_nodes , 2, sizeof(t_node));
    new->command_type = c_none;
    new->i_c_type =i_c_none;
    new->op_type =o_none;
    new->free = &node_free;
    new->add_child= &node_add_child;
    new->to_string = &node_to_string;
    return  (new);
}

t_bool node_add_child(t_node *this, t_node *new_node, int l_r)
{
    if (this == NULL || new_node == NULL)
        return (FALSE);
    if (l_r)
        return    this->left_nodes.push(&this->left_nodes, new_node, sizeof(t_node));
    return  this->right_nodes.push(&this->left_nodes, new_node, sizeof(t_node));
}

void  node_free(t_node *this)
{

}
void node_to_string(t_node *this)
{

}