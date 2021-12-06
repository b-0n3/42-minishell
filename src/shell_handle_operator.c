/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_handle_operator.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 06:41:36 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 06:45:48 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_mood;

t_bool	inject_command(t_node *head, t_token *token)
{
	t_node	*tmp;

	tmp = head;
	while (tmp->left != NULL && tmp->left->need_a_file(tmp->left))
		tmp = tmp->left;
	if (tmp->left != NULL && tmp->left->word_type == command)
	{
		tmp->left->args.push(&tmp->left->args, token, sizeof(t_token));
		return (TRUE);
	}
	else
	{
		tmp->left = token->to_node(token);
		tmp->left->parent = tmp;
		tmp->left->isleft = TRUE;
	}
	return (FALSE);
}

void	add_file(t_shell *this, t_node *head, t_token *token)
{
	if (this->parsing_error != NULL)
	{
		token->free(token);
		return ;
	}
	if (head->op_type == redirection)
	{
		if (head->output_file == NULL)
		{
			head->output_file = token->to_file(token);
			if (head->output_file->open(head->output_file,
					O_WRONLY | O_CREAT | O_TRUNC, 0644))
				this->parsing_error = strdup(head->output_file->uri);
		}
		else if (!inject_command(head, token))
			token->free(token);
	}
	else if (head->op_type == append)
	{
		if (head->output_file == NULL)
		{
			head->output_file = token->to_file(token);
			if (head->output_file->open(head->output_file,
					O_CREAT | O_APPEND | O_WRONLY, 0644))
				this->parsing_error = strdup(head->output_file->uri);
		}
		else if (!inject_command(head, token))
			token->free(token);
	}
	else if (head->op_type == input)
	{
		if (head->input_file == NULL)
		{
			head->input_file = token->to_file(token);
			if (head->input_file->open(head->input_file, O_RDONLY, -1))
				this->parsing_error = strdup(head->input_file->uri);
		}
		else if (!inject_command(head, token))
			token->free(token);
	}
	else
	{
		if (head->output_file == NULL)
		{
			head->eof = strdup(token->value);
			head->output_file = new_file("/tmp/B0N3_HEREDOC");
			if (head->output_file->open(head->output_file,
					O_WRONLY | O_TRUNC | O_CREAT, 0644))
				this->parsing_error = strdup(head->output_file->uri);
		}
		else if (!inject_command(head, token))
			token->free(token);
	}
}

// cat hell | wc -l >> f1 | grep hell.js > f1
t_node	*handle_operator(t_shell *this, t_token *token, t_node *head)
{
	t_node	*operator;

	if (token == NULL)
		return (head);
	operator = token->to_node(token);
	if (operator->op_type == pipeline)
	{
		if (head != NULL)
		{
			operator->left = head;
			head->isleft = TRUE;
			head->parent = operator;
			head = operator;
		}
		else
			head = operator;
	}
	else if (head != NULL)
	{
		if (head->op_type == pipeline)
		{
			if (head->right != NULL)
			{
				operator->left = head->right;
				head->right->isleft = TRUE;
				head->right->parent = operator;
				head->right = operator;
				operator->parent = head;
				operator->isleft = FALSE;
			}
			else
			{
				operator->isleft = FALSE;
				head->right = operator;
				head->right->parent = head;
				operator->parent = head;
			}
		}
		else
		{
			operator->left = head;
			head->isleft = TRUE;
			head->parent = operator;
			head = operator;
		}
	}
	else
		head = operator;
	return (head);
}
