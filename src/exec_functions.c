/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 06:12:14 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 06:16:31 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by Abdelouahad Ait hamd on 12/1/21.
//
#include <unistd.h>
#include "minishell.h"
#include <errno.h>

t_string	*node_to_execv_args(t_node *node)
{
	t_string			*ret;
	t_array_iterator	*iter;
	int					i;

	if (node == NULL || node->word_type == w_t_none)
		return (NULL);
	ret = malloc(sizeof(char *) * (node->args.index + 2));
	i = 0;
	iter = node->args.iterator(&node->args);
	ret[i] = strdup(node->value);
	i++;
	while (iter->has_next(iter))
	{
		ret[i] = strdup(((t_token *)iter->next(iter))->value);
		i++;
	}
	ret[i] = NULL;
	return (ret);
}

extern char	**environ;

void	exec_other(t_shell *this, t_node *node)
{
	t_string	*cmd;
	t_node		*tmp;

	cmd = node_to_execv_args(node);
	if (node->parent != NULL)
	{
		if (node->isleft)
		{
			if (node->parent->op_type == pipeline)
			{
				close(node->parent->p[0]);
				dup2(node->parent->p[1], STDOUT_FILENO);
				close(node->parent->p[1]);
			}
			else if (node->parent->op_type == input)
			{
				dup2(node->parent->input_file->fd, STDIN_FILENO);
				close(node->parent->input_file->fd);
			}
			else if (node->parent->op_type == heredoc)
			{
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
						while (tmp->parent != NULL && tmp->need_a_file(tmp))
							tmp = tmp->parent;
						dup2(tmp->output_file->fd, STDOUT_FILENO);
						close(tmp->output_file->fd);
					}
				}
			}
			else if (node->parent->op_type == redirection
				|| node->parent->op_type == append)
			{
				tmp = node->parent;
				while (tmp->parent != NULL
					&& tmp->parent->need_a_file(tmp->parent))
				{
					fprintf(stderr, "loop %s \n", tmp->output_file->uri);
					tmp = tmp->parent;
				}
				if (tmp->parent != NULL && tmp->parent->op_type == pipeline)
				{
					fprintf(stderr, "sadfsadf %s \n", tmp->parent->value);
					close(tmp->parent->p[1]);
					dup2(tmp->parent->p[0], STDIN_FILENO);
					close(tmp->parent->p[0]);
				}
				fprintf(stderr, "losadfop %s \n", tmp->output_file->uri);
				dup2(tmp->output_file->fd, STDOUT_FILENO);
				//  close(tmp->output_file->fd);
			}
		}
		else
		{
			close(node->parent->p[1]);
			dup2(node->parent->p[0], STDIN_FILENO);
			close(node->parent->p[0]);
			if (node->parent->parent != NULL)
			{
				close(node->parent->parent->p[0]);
				dup2(node->parent->parent->p[1], STDOUT_FILENO);
				close(node->parent->parent->p[1]);
			}
		}
	}
	find_command_in_path(this, node);
	if (execve(node->value, cmd, this->env_to_arr(this)) == -1)
	{
		if (errno != 13)
		{
			printf("minishell: %s command not found\n", node->value);
			exit(127);
		}
		perror("minishell");
		exit(126);
	}
}
