/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_p2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 03:43:12 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 03:54:03 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_env(t_shell *this, t_node *node)
{
	printf("env command \n");
}

void	exec_exit(t_shell *this, t_node *node)
{
	printf("exit command \n");
}

t_string	find_command_in_path(t_shell *this, t_node *cmd)
{
	t_string			path_variable;
	t_string			path;
	t_array_iterator	*iter;
	struct stat			sb;

	if (this == NULL || cmd == NULL || cmd->value == NULL)
		return (NULL);
	if (strchr(cmd->value, '/') != NULL)
		return (strdup(cmd->value));
			path_variable = (t_string)this->env.find_by_key(this->env, "PATH");
	if (path_variable == NULL)
		return (NULL);
	iter = split(path_variable, ':');
	while (iter->has_next(iter))
	{
		path = iter->next(iter);
		if (path != NULL)
		{
			path_variable = strdup(path);
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
