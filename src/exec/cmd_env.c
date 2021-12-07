/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 01:48:11 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/07 02:04:03 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_mood;

void	print_env(void *item)
{
	t_key_map	*map;

	map = (t_key_map *) item;
	if (map->value != NULL)
	{
		write(STDOUT_FILENO, map->key, ft_strlen(map->key));
		write(STDOUT_FILENO, "=", 1);
		write(STDOUT_FILENO, map->value, ft_strlen((char *) map->value));
		write(STDOUT_FILENO, "\n", 1);
	}
}

void	cmd_env(t_shell *this)
{
	this->env.foreach(&this->env, &print_env);
	if (g_mood == 1)
		exit(0);
}

t_bool	env_cond(void *item, void *item2)
{
	t_string	key;
	t_key_map	*map;

	map = (t_key_map *) item;
	key = (t_string) item2;
	if (map == NULL)
		return (FALSE);
	if (map->key == NULL || key == NULL)
		return (FALSE);
	return (!ft_strcmp((char *)map->key, key));
}

void	unset_token(t_shell *this, t_token *token)
{
	t_key_map	*m;
	size_t		i;

	i = 0;
	if (token->value != NULL)
	{
		while (i < this->env.index)
		{
			m = this->env.get(&this->env, i);
			if (m != NULL)
			{
				if (env_cond(m, token->value))
				{
					this->env.arr[i] = NULL;
				}
			}
			i++;
		}
	}
}

void	cmd_unset(t_shell *this, t_node *head)
{
	t_array_iterator	*iterator;
	t_token				*token;

	iterator = head->args.iterator(&head->args);
	while (iterator->has_next(iterator))
	{
		token = (t_token *) iterator->next(iterator);
		unset_token(this, token);
	}
	iterator->free(iterator, NULL);
}
