/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 03:09:27 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 09:49:53 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	checkdir(char *dirname)
{
	DIR	*dir;

	dir = opendir(dirname);
	if (dir)
	{
		closedir(dir);
		return (1);
	}
	else
	{
		write(STDOUT_FILENO, "cd: no such file or directory: ", 32);
		write(STDOUT_FILENO, dirname, sizeof(dirname));
		write(STDOUT_FILENO, " \n", 3);
		exit(1);
	}
}

int	edit_env(t_shell *this, char *dest_path, char *old_path)
{
	char	tmp[1024];

	if (chdir(dest_path) < 0)
	{
		write(STDOUT_FILENO, "cd: no such file or directory: ", 32);
		write(STDOUT_FILENO, dest_path, sizeof(dest_path));
		write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
	if (dest_path == NULL)
		dest_path = strdup("");
	if (old_path == NULL)
		old_path = strdup("");
	getcwd(tmp, 1024);
	this->env.replace_by_key(&this->env, "PWD", strdup(tmp), &free);
	//this->env.replace_by_key(&this->env, "OLDPWD", strdup(old_path), &free);
	return (1);
}

void	cmd_cd(t_shell *this, t_node *head)
{
	t_string	dest_path;
	t_string	oldpath;

	if (head->args.index > 0)
		dest_path = (char *)((t_token *)head->args.get(&head->args, 0))->value;
	else
		dest_path = NULL;
	oldpath = (char *) this->env.find_by_key(this->env, "PWD");
	if (dest_path == NULL)
		dest_path = this->env.find_by_key(this->env, "HOME");
	if (dest_path == NULL)
		return ;
	edit_env(this, dest_path, oldpath);
}

void	cmd_pwd(t_shell *this)
{
	char	*tmp;

	tmp = this->env.find_by_key(this->env, "PWD");
	if (tmp == NULL)
		write(STDOUT_FILENO, "(null)", 6);
	else
	{
		write(STDOUT_FILENO, tmp, strlen(tmp));
		write(STDOUT_FILENO, "\n", 1);
	}
}
