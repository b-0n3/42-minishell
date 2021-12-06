/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 10:04:13 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 10:59:44 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_env(t_shell *this, t_node *head)
{
	size_t			i;
	t_array_list	*m;
	t_key_map		*sw;

	m = (t_array_list *) &this->env;
	i = 0;
	while (i < m->index)
	{
		if (m->arr[i] != NULL)
		{
			sw = (t_key_map *) m->arr[i];
			write(STDOUT_FILENO, sw->key, sizeof(sw->key));
			write(STDOUT_FILENO, "=", 1);
			write(STDOUT_FILENO, sw->value, sizeof(sw->value));
			write(STDOUT_FILENO, "\n", 1);
		}
		i++;
	}
}
