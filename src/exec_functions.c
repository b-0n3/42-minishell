//
// Created by Abdelouahad Ait hamd on 12/1/21.
//
#include <unistd.h>
#include "minishell.h"

t_string	find_command_in_path(t_shell *this, t_node *cmd)
{
	t_string			path_variable;
	t_string			path;
	t_array_iterator	*iter;
	struct stat			sb;

	if (this == NULL || cmd == NULL || cmd->value == NULL)
		return (NULL);
	path_variable = (t_string) this->env.find_by_key(this->env, "PATH");
	if (path_variable == NULL)
		return (NULL);
	iter = split(path_variable, ':');
	while (iter->has_next(iter))
	{
		path = iter->next(iter);
		if (path != NULL)
		{
			if (!end_with(path, '/'))
			{
				path_variable = ft_strjoin(path, "/");
				path = path_variable;
			}
			path_variable = ft_strjoin(path, cmd->value);
			if (stat(path_variable, &sb) == 0)
			{
				free(cmd->value);
				cmd->value = path_variable;
				return (cmd->value);
			}
		}
	}
	return (NULL);
}

t_string	*node_to_execv_args(t_node *node)
{
	t_string			*ret;
	t_array_iterator	*iter;
	int					i;

	if (node == NULL || node->word_type == w_t_none)
		return (NULL);
	ret = malloc(sizeof(char *) + (node->args.index) + 1);
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

void	close_end_dup(int fd1, int fd2)
{
	close(fd1);
	dup2(fd2, STDOUT_FILENO);
	close(fd2);
}

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
				close_end_dup(node->parent->p[0], node->parent->p[1]);
			else
			{
				tmp = node->parent;
				while (tmp->parent != NULL && tmp->need_a_file(tmp))
					tmp = tmp->parent;
				dup2(tmp->output_file->fd, STDOUT_FILENO);
				close(tmp->output_file->fd);
			}
		}
		else
		{
			close_end_dup(node->parent->p[1], node->parent->p[0]);
			if (node->parent->parent != NULL)
				close_end_dup(node->parent->p[0], node->parent->p[1]);
		}
	}
	if (find_command_in_path(this, node) == NULL)
		return ;
	execve(node->value, cmd, environ);
}
