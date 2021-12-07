/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 02:03:28 by aait-ham          #+#    #+#             */
/*   Updated: 2021/12/07 02:03:31 by aait-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_echo(t_shell *this, t_node *node)
{
	int	in;
	int	out;

	out = dup(STDOUT_FILENO);
	in = dup(STDIN_FILENO);
	init_fds(node);
	cmd_echo(node);
	dup2(out, STDOUT_FILENO);
	dup2(in, STDIN_FILENO);
	close(in);
	close(out);
	this->exit_code = 0;
}

void	exec_cd(t_shell *this, t_node *node)
{
	int	in;
	int	out;

	out = dup(STDOUT_FILENO);
	in = dup(STDIN_FILENO);
	init_fds(node);
	cmd_cd(this, node);
	dup2(out, STDOUT_FILENO);
	dup2(in, STDIN_FILENO);
	close(in);
	close(out);
}

void	exec_pwd(t_shell *this, t_node *node)
{
	int	in;
	int	out;

	out = dup(STDOUT_FILENO);
	in = dup(STDIN_FILENO);
	init_fds(node);
	cmd_pwd(this);
	dup2(out, STDOUT_FILENO);
	dup2(in, STDIN_FILENO);
	close(in);
	close(out);
	this->exit_code = 0;
}

void	exec_export(t_shell *this, t_node *node)
{
	int	in;
	int	out;

	out = dup(STDOUT_FILENO);
	in = dup(STDIN_FILENO);
	init_fds(node);
	cmd_export(this, node);
	dup2(out, STDOUT_FILENO);
	dup2(in, STDIN_FILENO);
	close(in);
	close(out);
}

void	exec_unset(t_shell *this, t_node *node)
{
	int	in;
	int	out;

	out = dup(STDOUT_FILENO);
	in = dup(STDIN_FILENO);
	init_fds(node);
	cmd_unset(this, node);
	dup2(out, STDOUT_FILENO);
	dup2(in, STDIN_FILENO);
	close(in);
	close(out);
	this->exit_code = 0;
}
