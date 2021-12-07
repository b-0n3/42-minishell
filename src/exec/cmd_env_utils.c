/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 01:51:53 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/07 02:03:31 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_mood;

void	print_export(void *item)
{
	t_key_map	*map;

	map = (t_key_map *) item;
	if (map == NULL)
		return ;
	write(STDOUT_FILENO, "declare -x ", 11);
	write(STDOUT_FILENO, map->key, ft_strlen(map->key));
	if (map->value != NULL)
	{
		write(STDOUT_FILENO, "=\"", 2);
		write(STDOUT_FILENO, map->value, ft_strlen((char *) map->value));
		write(STDOUT_FILENO, "\"", 1);
	}
	write(STDOUT_FILENO, "\n", 1);
}

t_bool	check_ident(t_key_map *map)
{
	int	i;

	i = 0;
	if (map == NULL || map->key == NULL)
		return (FALSE);
	while (map->key[i] == ' ')
		i++;
	return (ft_isdigit(map->key[i])
		|| (map->key[i] != '_' && !ft_isalpha(map->key[i])));
}

t_bool	end_with_(t_string str, char c)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (str[i] = c);
}

void	export_cmd_free(t_shell *this, t_key_map *map)
{
	this->exit_code = 1;
	printf("minishell: export: not a valid identifier\n");
	my_free(map->key);
	my_free(map);
	my_free(map->value);
	if (g_mood == 1)
		exit(1);
}

void	cmd_export(t_shell *this, t_node *node)
{
	t_key_map	*map;
	t_string	old_val;

	old_val = NULL;
	if (node->args.index == 0)
		this->env.foreach(&this->env, &print_export);
	else
	{
		map = env_to_key_map(((t_token *)
					node->args.get(&node->args, 0))->value);
		if (check_ident(map))
		{
			export_cmd_free(this, map);
			return ;
		}
		this->env.replace_by_key(&this->env, map->key, map->value, &free);
		free(map->key);
		free(map);
	}
	if (g_mood == 1)
		exit(0);
	this->exit_code = 0;
}
