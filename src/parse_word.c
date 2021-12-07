/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 03:38:00 by aait-ham          #+#    #+#             */
/*   Updated: 2021/12/07 03:38:02 by aait-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void parse_word_pipeline(t_node *head, t_node *com)
{
    if (head->left == NULL) {
                head->left = com;
                com->isleft = TRUE;
            }
              else {
                head->right = com;
                com->isleft = FALSE;
            }
                com->parent = head;
}
t_node  *parse_args(t_node *head, t_token *token, t_node *com)
{
    while (1)
	{
			token = pre_get_next_token(this);
            if (token != NULL)
               token->expand(token,this->exit_code, this->env);
			if (token == NULL || token->type != word)
				return handle_operator(this, token ,head);
			com->args.push(&com->args, token , sizeof(t_token));
		}
    return head;
}

t_node *handle_word(t_shell *this, t_token *token ,t_node *head)
{
	t_node *com;

    token->expand(token,this->exit_code, this->env);
	if (head != NULL && head->right != NULL
    && head->right->need_a_file(head->right))
        add_file(this,head->right, token);
	else if (head != NULL && head->need_a_file(head))
		add_file(this,head, token);
	else if (head == NULL || head->op_type == pipeline  )
	{
		com = token->to_node(token);
		token->free(token);
		if (head != NULL)
            parse_word_pipeline(head, com);
        else
			head = com;
		return (parse_args(head, token ,com));
	}
	return head;
}
