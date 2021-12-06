/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 07:17:13 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 07:18:33 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by Abdelouahad Ait hamd on 11/8/21.
//

#include "minishell.h"

extern int	g_mood;

t_string	*shell_env_to_arr(t_shell *this)
{
	t_array_iterator	*iter;
	t_string			*arr;
	int					i;

	i = 0;
	if (this == NULL)
		return (NULL);
	arr = malloc(sizeof (char *) *(this->env.index + 1));
	iter = this->env.iterator(&this->env);
	while (iter->has_next(iter))
	{
		arr[i] = iter->do_on_next(iter, &map_to_string);
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

void	shell_loop(t_shell *this)
{
	t_string	line;

	line = readline("IM->B0N3$>");
	while (line != NULL && strcmp(line, "exit") != 0)
	{
		this->init(this, line);
		if (this->parse(this))
		{
			this->execute(this);
		}
		else
			perror(this->parsing_error);
		// execute command
		add_history(line);
		line = readline("IM->B0N3$>");
	}
	this->free(this);
	// @Todo: call exit
}

void	shell_free(t_shell *this)
{
	return ;
}
