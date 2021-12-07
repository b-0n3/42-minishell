/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 03:25:49 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/07 03:26:46 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_key_map(void *item)
{
	t_key_map	*map;

	map = (t_key_map *)map;
	if (map != NULL)
		my_free(map->key);
	my_free(map);
}

void	*map_to_string(void *item)
{
	t_key_map	*m;
	t_string	tmp;

	if (item == NULL)
		return (NULL);
	m = (t_key_map *) item;
	tmp = ft_strdup(m->key);
	tmp = ft_strjoin(tmp, "=");
	if (m->value != NULL)
		tmp = ft_strjoin(tmp, (char *)m->value);
	return (tmp);
}

t_string	*shell_env_to_arr(t_shell *this)
{
	t_array_iterator	*iter;
	t_string			*arr;
	t_string			ss;
	int					i;

	i = 0;
	if (this == NULL)
		return (NULL);
	arr = malloc(sizeof (char *) *(this->env.index + 1));
	iter = this->env.iterator(&this->env);
	while (iter->has_next(iter))
	{
		ss = iter->do_on_next(iter, &map_to_string);
		if (ss == NULL)
			continue ;
		arr[i] = ss;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
