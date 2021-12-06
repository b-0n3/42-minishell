/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 09:59:55 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 10:02:15 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_unset(t_shell *shell, t_node *head)
{
	t_array_list	this;
	t_key_map		*item;
	t_string		key;
	int				i;

	if (head->args.index > 0)
		key = (char *)((t_token *)head->args.get(&head->args, 0))->value;
	this = (t_array_list) shell->env;
	i = 0;
	if (this.is_map)
	{
		item = (t_key_map *) this.get(&this, i);
		while (item != NULL)
		{
			if (!strcmp(key, (const char *) item->key))
			{
				this.pop_i(&this, i);
			}
			i++;
			item = (t_key_map *) this.get(&this, i);
		}
	}
}
