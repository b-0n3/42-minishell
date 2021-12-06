/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 09:44:57 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 11:01:43 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_export(t_shell *this, t_node *head)
{
	t_array_iterator	*iterator;
	t_key_map			*map;

	if (head->args.index > 0)
		iterator = (char *)((t_token *)head->args.get(&head->args, 0))->value;
	else
		iterator = NULL;
	map = malloc(sizeof(t_key_map));
	if (map == NULL)
		return ;
	map->key = split_env(iterator);
	map->value = strdup(iterator + strlen(map->key) + 1);
	if ((char *) this->env.find_by_key(this->env, map->key) == NULL)
		this->env.push(&this, map, sizeof(t_key_map *));
	else
		this->env.replace_by_key(&this->env, map->key, map->value,
			strlen(map->value));
}
