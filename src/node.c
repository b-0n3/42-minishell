/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 02:46:15 by aait-ham          #+#    #+#             */
/*   Updated: 2021/12/07 02:46:21 by aait-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"

t_op_type find_operation_type(t_string value)
{
    if (ft_strcmp(value, "<<") == 0)
        return heredoc;
    if (ft_strcmp(value, "|") == 0)
        return pipeline;
    if (ft_strcmp(value, ">>") == 0)
        return append;
    if (ft_strcmp(value, "<") == 0)
        return input;
    if (ft_strcmp(value, ">") == 0)
        return redirection;
    return  o_t_none;
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
    new->eof = NULL;
    new->parent = NULL;
    new->right = NULL;
    new->need_a_file = &node_need_a_file;
    new->is_operation = &node_is_operation;
    new->free = &node_free;
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
    my_free(this->value);
    if (this->op_type == heredoc)
        unlink(this->output_file->uri);
    this->args.free(&this->args, &token_free);
    my_free(this->eof);
    if (this->output_file != NULL)
    {
        my_free(this->output_file->uri);
        my_free(this->output_file);
    }
    if (this->input_file != NULL)
    {
        my_free(this->input_file->uri);
        my_free(this->input_file);
    }

    free(this);
}
