/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 01:41:41 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/07 02:03:03 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_mood;

int	edit_env(t_shell *this, char *dest_path)
{
	char	tmp[1024];

	if (chdir(dest_path) < 0)
	{
		perror("cd : ");
		write(STDOUT_FILENO, dest_path, ft_strlen(dest_path));
		write(STDOUT_FILENO, "\n", 1);
		return (1);
	}
	this->env.replace_by_key(&this->env, "PWD",
		ft_strdup(getcwd(tmp, 1024)), &free);
	return (0);
}

void	cmd_cd(t_shell *this, t_node *head)
{
	t_string			dest_path;

	if (head->args.index > 0)
		dest_path = (char *)((t_token *) head->args.get(&head->args, 0))->value;
	else
		dest_path = NULL;
	if (dest_path == NULL)
		dest_path = this->env.find_by_key(this->env, "HOME");
	if (dest_path == NULL)
		return ;
	this->exit_code = edit_env(this, dest_path);
	if (g_mood == 1)
		exit(this->exit_code);
}

void	cmd_pwd(t_shell *this)
{
	char	*tmp;

	tmp = (char *) this->env.find_by_key(this->env, "PWD");
	if (tmp == NULL)
		write(STDOUT_FILENO, "(null)", 6);
	else
	{
		write(STDOUT_FILENO, tmp, ft_strlen(tmp));
		write(STDOUT_FILENO, "\n", 1);
	}
	if (g_mood == 1)
		exit(0);
}
