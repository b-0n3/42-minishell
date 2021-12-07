/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 03:42:09 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/07 03:43:19 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_head_pipeline(t_node **head, t_node *operator)
{
	if ((*head)->right != NULL)
	{
		operator->left = (*head)->right;
		(*head)->right->isleft = TRUE;
		(*head)->right->parent = operator;
		(*head)->right = operator;
		operator->parent = (*head);
		operator->isleft = FALSE;
	}
	else
	{
		operator->isleft = FALSE;
		(*head)->right = operator;
		(*head)->right->parent = (*head);
		operator->parent = (*head);
	}
}

void	parse_pipeline(t_node **head, t_node *operator)
{
	if (*head != NULL)
	{
		operator->left = *head;
		(*head)->isleft = TRUE;
		(*head)->parent = operator;
		*head = operator;
	}
	else
		(*head) = operator;
}

t_node	*handle_operator(t_shell *this, t_token *token, t_node **head)
{
	t_node	*operator;

	this = NULL;
	if (token == NULL)
		return (*head);
	operator = token->to_node(token);
	if (operator->op_type == pipeline)
		parse_pipeline(head, operator);
	else if (head != NULL)
	{
		if ((*head)->op_type == pipeline)
			parse_head_pipeline(head, operator);
		else
		{
			operator->left = *head;
			(*head)->isleft = TRUE;
			(*head)->parent = operator;
			*head = operator;
		}
	}
	else
		(*head) = operator;
	token->free(token);
	return ((*head));
}
