/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_init_fds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 02:44:20 by aait-ham          #+#    #+#             */
/*   Updated: 2021/12/07 03:11:01 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	link_left_pipe(t_node *node)
{
	t_node	*tmp;

	close(node->parent->p[0]);
	dup2(node->parent->p[1], STDOUT_FILENO);
	close(node->parent->p[1]);
	tmp = node->parent;
	while (tmp->parent != NULL)
	{
		if (tmp->parent->op_type == pipeline)
		{
			close(tmp->parent->p[0]);
			close(tmp->parent->p[1]);
		}
		tmp = tmp->parent;
	}
}

void	link_here_doc(t_node *node)
{
	t_node	*tmp;

	dup2(node->parent->output_file->fd, STDIN_FILENO);
	close(node->parent->output_file->fd);
	if (node->parent->parent != NULL)
	{
		if (node->parent->parent->op_type == pipeline)
		{
			close(node->parent->parent->p[0]);
			dup2(node->parent->parent->p[1], STDOUT_FILENO);
			close(node->parent->parent->p[1]);
		}
		else if (node->parent->parent->op_type == redirection
			|| node->parent->parent->op_type == append)
		{
			tmp = node->parent->parent;
			while (tmp->parent != NULL && tmp->parent->need_a_file(tmp->parent))
				tmp = tmp->parent;
			dup2(tmp->output_file->fd, STDOUT_FILENO);
			close(tmp->output_file->fd);
		}
	}
}

void	close_for_right(t_node *node)
{
	t_node	*tmp;

	close(node->parent->p[1]);
	dup2(node->parent->p[0], STDIN_FILENO);
	close(node->parent->p[0]);
	if (node->parent->parent != NULL
		&& node->parent->parent->op_type == pipeline)
	{
		close(node->parent->parent->p[0]);
		dup2(node->parent->parent->p[1], STDOUT_FILENO);
		close(node->parent->parent->p[1]);
		tmp = node->parent;
		while (tmp->parent != NULL)
		{
			if (tmp->parent->op_type == pipeline)
			{
				close(tmp->parent->p[0]);
				close(tmp->parent->p[1]);
			}
			tmp = tmp->parent;
		}
	}
}

void	link_redirection(t_node *node)
{
	t_node	*tmp;

	tmp = node->parent;
	while (tmp->parent != NULL && tmp->parent->need_a_file(tmp->parent))
	{
		tmp = tmp->parent;
	}
	if (tmp->parent != NULL && tmp->parent->op_type == pipeline)
	{
		close(tmp->parent->p[1]);
		dup2(tmp->parent->p[0], STDIN_FILENO);
		close(tmp->parent->p[0]);
	}
	dup2(tmp->output_file->fd, STDOUT_FILENO);
	close(tmp->output_file->fd);
}

void	init_fds(t_node *node)
{
	if (node->parent != NULL)
	{
		if (node->isleft)
		{
			if (node->parent->op_type == pipeline)
				link_left_pipe(node);
			else if (node->parent->op_type == input)
			{
				dup2(node->parent->input_file->fd, STDIN_FILENO);
				close(node->parent->input_file->fd);
			}
			else if (node->parent->op_type == heredoc)
				link_here_doc(node);
			else if (node->parent->op_type == redirection
				|| node->parent->op_type == append)
				link_redirection(node);
		}
		else
			close_for_right(node);
	}
}
