/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entry_point.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 01:53:18 by aait-ham          #+#    #+#             */
/*   Updated: 2021/12/07 01:53:21 by aait-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_mood;

void	quit_handler(int num)
{
	if (g_mood == 0)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	main(int argc, t_string *argv, t_string *env)
{
	t_shell	shell;

	argc = 1;
	argv = NULL;
	g_mood = 0;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &quit_handler);
	create_shell(&shell, env);
	shell.env.is_map = TRUE;
	shell.loop(&shell);
}
