/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 06:18:25 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 10:05:45 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by Abdelouahad Ait hamd on 12/2/21.
//
#include "minishell.h"

void	init_exec_builtins(t_shell *this)
{
	if (this != NULL)
	{
		new_array_list(&this->exec_pool, 10, sizeof(t_key_map));
		this->exec_pool.is_map = TRUE;
		this->exec_pool.push(&this->exec_pool,
			new_func_map("echo", &exec_echo), sizeof (t_key_map));
		this->exec_pool.push(&this->exec_pool,
			new_func_map("cd", &exec_cd), sizeof (t_key_map));
		this->exec_pool.push(&this->exec_pool,
			new_func_map("pwd", &exec_pwd), sizeof (t_key_map));
		this->exec_pool.push(&this->exec_pool,
			new_func_map("export", &exec_export), sizeof (t_key_map));
		this->exec_pool.push(&this->exec_pool,
			new_func_map("unset", &exec_unset), sizeof (t_key_map));
		this->exec_pool.push(&this->exec_pool,
			new_func_map("env", &exec_env), sizeof (t_key_map));
		this->exec_pool.push(&this->exec_pool,
			new_func_map("exit", &exec_exit), sizeof (t_key_map));
	}
}

t_key_map	*new_func_map(t_string key, exec_v *func)
{
	t_key_map	*this;

	this = malloc(sizeof(t_func_map));
	if (this == NULL)
		return (NULL);
	this->key = strdup(key);
	this->value = func;
	return (this);
}
