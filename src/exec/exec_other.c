/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_other.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 02:10:04 by aait-ham          #+#    #+#             */
/*   Updated: 2021/12/07 02:10:06 by aait-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_string	find_command(t_array_iterator *iter,
	t_string path_variable, t_string path, t_node *cmd)
{
	struct stat	sb;

	while (iter->has_next(iter))
	{
		path = iter->next(iter);
		if (path != NULL)
		{
			path_variable = ft_strdup(path);
			path_variable = ft_strjoin(path_variable, "/");
			path_variable = ft_strjoin(path_variable, cmd->value);
			if (stat(path_variable, &sb) == 0)
			{
				free(cmd->value);
				cmd->value = path_variable;
				iter->free(iter, &free);
				return (cmd->value);
			}
			free(path_variable);
		}
	}
	iter->free(iter, &free);
	return (NULL);
}

t_string	find_command_in_path(t_shell *this, t_node *cmd)
{
	t_string			path_variable;
	t_string			path;
	t_array_iterator	*iter;

	path = NULL;
	if (this == NULL || cmd == NULL || cmd->value == NULL)
		return (NULL);
	if (ft_strchr(cmd->value, '/') != NULL)
		return (ft_strdup(cmd->value));
	path_variable = (t_string)this->env.find_by_key(this->env, "PATH");
	if (path_variable == NULL)
		return (NULL);
	iter = split(path_variable, ':');
	return (find_command(iter, path_variable, path, cmd));
}

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
	ret[i] = ft_strdup(node->value);
	i++;
	while (iter->has_next(iter))
	{
		ret[i] = ft_strdup(((t_token *)iter->next(iter))->value);
		i++;
	}
	iter->free(iter, NULL);
	ret[i] = NULL;
	return (ret);
}

void	exec_other(t_shell *this, t_node *node)
{
	t_string	*cmd;

	cmd = node_to_execv_args(node);
	init_fds(node);
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