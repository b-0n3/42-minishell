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
