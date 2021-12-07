/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_add.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 03:12:04 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/07 03:15:24 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	add_file_heredoc(t_shell *this, t_node *head, t_token *token)
{
	char	*heredoc;
	char	*tmp;

	if (head->output_file == NULL)
	{
		tmp = ft_strdup("/tmp/B0N3_HEREDOC");
		heredoc = ft_itoa(this->h_d_index++);
		tmp = ft_strjoin(tmp, heredoc);
		head->eof = ft_strdup(token->value);
		head->output_file = new_file(tmp);
		free(heredoc);
		free(tmp);
		if (head->output_file->open(head->output_file,
				O_WRONLY | O_TRUNC | O_CREAT, 0644))
			this->parsing_error = strdup(head->output_file->uri);
		token->free(token);
	}
	else if (!inject_command(head, token))
		token->free(token);
}

void	add_file_input(t_shell *this, t_node *head, t_token *token)
{
	if (head->input_file == NULL)
	{
		head->input_file = token->to_file(token);
		if (head->input_file->open(head->input_file, O_RDONLY, -1))
			this->parsing_error = strdup(head->input_file->uri);
		token->free(token);
	}
	else if (!inject_command(head, token))
		token->free(token);
}

void	add_file_append(t_shell *this, t_node *head, t_token *token)
{
	if (head->output_file == NULL)
	{
		head->output_file = token->to_file(token);
		if (head->output_file->open(head->output_file,
				O_CREAT | O_APPEND | O_WRONLY, 0644))
			this->parsing_error = strdup(head->output_file->uri);
		token->free(token);
	}
	else if (!inject_command(head, token))
		token->free(token);
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
			token->free(token);
		}
		else if (!inject_command(head, token))
			token->free(token);
	}
	else if (head->op_type == append)
		add_file_append(this, head, token);
	else if (head->op_type == input)
		add_file_input(this, head, token);
	else
		add_file_heredoc(this, head, token);
}
