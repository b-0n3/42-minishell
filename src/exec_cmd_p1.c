/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_p1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 03:41:15 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 03:46:55 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_echo(t_shell *this, t_node *node)
{
	printf("echo command \n");
}

void	exec_cd(t_shell *this, t_node *node)
{
	cmd_cd(this, node);
}

void	exec_pwd(t_shell *this, t_node *node)
{
	cmd_pwd(this);
	node = node;
}

void	exec_export(t_shell *this, t_node *node)
{
	printf("export command \n");
}

void	exec_unset(t_shell *this, t_node *node)
{
	printf("unset command \n");
}
