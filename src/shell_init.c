/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 03:31:23 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/07 03:31:51 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_shell(t_shell *this, t_string *env)
{
	this->parse = &shell_parse;
	new_array_list(&this->env, 10, sizeof(t_key_map));
	push_env(&this->env, env);
	this->fresh = TRUE;
	this->init = &init_shell;
	this->get_next_token = &shell_get_next_token;
	this->has_next_token = &has_next_token;
	this->unclosed = &shell_quot_unclosed;
	this->execute = &shell_execute;
	this->loop = &shell_loop;
	this->env_to_arr = &shell_env_to_arr;
	this->free = &shell_free;
	this->exit_code = 0;
	init_exec_builtins(this);
}

void	init_shell(t_shell *this, t_string line)
{
	if (this != NULL)
	{
		if (!this->fresh)
		{
			if (this->parsing_error != NULL)
				free(this->parsing_error);
			node_free(this->head);
		}
		this->cursor = 0;
		this->l_cursor = 0;
		this->h_d_index = 0;
		this->fresh = 0;
		this->commmand = line;
		while (this->commmand[this->cursor] == ' ')
		{
			this->cursor++;
			this->l_cursor++;
		}
		this->parsing_error = NULL;
		this->command_len = strlen(this->commmand);
		this->dqout = FALSE;
		this->quot = FALSE;
	}
}

void	shell_free(t_shell *this)
{
	this->exec_pool.free(&this->exec_pool, &free);
	node_free(this->head);
	system("leaks minishell");
}
