//
// Created by Abdelouahad Ait hamd on 11/10/21.
//

#include "minishell.h"

t_op_type find_operation_type(t_string value)
{
    if (strcmp(value, "<<") == 0)
        return heredoc;
    if (strcmp(value, "|") == 0)
        return pipeline;
    if (strcmp(value, ">>") == 0)
        return append;
    if (strcmp(value, "<") == 0)
        return input;
    if (strcmp(value, ">") == 0)
        return redirection;
    return  o_t_none;
}

t_file  *token_to_file(t_token *this) {
    if (this == NULL)
            return NULL;
    return new_file(this->value);
}

t_file *new_file(t_string uri)
{
    t_file *file;

    file = malloc(sizeof (t_file));
    file->uri = strdup(uri);
    file->exception = 0;
    file->open = &file_open;
    file->fd = -1;
    return  file;
}

t_node  *new_node()
{
    t_node  *new;

    new  = malloc(sizeof(t_node));
    if (new == NULL)
        return (NULL);
    new_array_list(&new->args, 2, sizeof(t_token));

    new->command_type = c_none;
    new->i_c_type =i_c_none;
    new->word_type = w_t_none;
    new->op_type = o_t_none;
    new->input_file = NULL;
    new->output_file = NULL;
    new->left = NULL;
    new->parent = NULL;
    new->right = NULL;
    new->need_a_file = &node_need_a_file;
    new->is_operation = &node_is_operation;
    new->free = &node_free;
    new->to_string = &node_to_string;
    return  (new);
}

t_bool node_is_operation(t_node *this)
{

    if (this != NULL)
    {
        return  (this->op_type != o_t_none);
    }
    return FALSE;
}


t_bool node_need_a_file(t_node *this)
{
    if (this == NULL)
        return (FALSE);
    return (this->op_type != pipeline && this->op_type != o_t_none);
}

void  node_free(t_node *this)
{
    if (this == NULL)
        return;
    node_free(this->left);
    node_free(this->right);
    free(this->value);
    this->args.free(&this->args, &token_free);

    free(this);
}

void node_to_string(t_node *this)
{

}