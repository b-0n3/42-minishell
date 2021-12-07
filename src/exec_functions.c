/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 01:53:34 by aait-ham          #+#    #+#             */
/*   Updated: 2021/12/07 01:54:19 by aait-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_env(t_shell *this, t_node *node)
{
	int	in;
	int	out;

	out = dup(STDOUT_FILENO);
	in = dup(STDIN_FILENO);
	init_fds(node);
	cmd_env(this);
	dup2(out, STDOUT_FILENO);
	dup2(in, STDIN_FILENO);
	close(in);
	close(out);
	this->exit_code = 0;
}

void	exec_exit(t_shell *this, t_node *node)
{
	int	in;
	int	out;

	out = dup(STDOUT_FILENO);
	in = dup(STDIN_FILENO);
	init_fds(node);
	cmd_exit(this, node);
	dup2(out, STDOUT_FILENO);
	dup2(in, STDIN_FILENO);
	close(in);
	close(out);
}
