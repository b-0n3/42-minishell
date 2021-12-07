/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 03:28:04 by aait-ham          #+#    #+#             */
/*   Updated: 2021/12/07 03:28:06 by aait-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool has_next_token(t_shell *this)
{
	return this->cursor <this->command_len;
}

t_token *pre_get_next_token(t_shell *this)
{
	t_token *token;

	token = this->get_next_token(this);
	return token;
}

void *str_to_token(void *str)
{
    return new_token(strdup((t_string )str), word);
}

t_node *token_to_node(t_token *this)
{
	t_node  *node;
    t_array_iterator  *iter;

	node = new_node();
	if (node == NULL)
		return (NULL);
	node->value = strdup(this->value);
	if (this->type == word) {
        node->word_type = command;
        if (this->start_with != '\"' && this->start_with != '\'') {
            iter = split(this->value, ' ');
            if (iter->list->index > 1) {
                free(node->value);
                node->value = strdup((char *)iter->next(iter));
                while(iter->has_next(iter))
                  node->args.push(&node->args, iter->do_on_next(iter, &str_to_token), sizeof(t_token));
            }
            iter->free(iter, &free);
        }
    }
    else
		node->op_type = find_operation_type(this->value);
	return node;
}
