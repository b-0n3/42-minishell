/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   env_parse.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: am-khant <am-khant@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2021/12/07 02:49:28 by aait-ham		  #+#	#+#			 */
/*   Updated: 2021/12/07 03:09:28 by am-khant		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

t_string	split_env(t_string env)
{
	int	i;

	i = 0;
	while (env[i] != '\0' && env[i] != '=')
		i++;
	return (ft_strndup(env, i));
}

t_key_map	*env_to_key_map(t_string str)
{
	t_key_map	*map;

	map = malloc(sizeof(t_key_map));
	if (map == NULL)
		return (NULL);
	map->key = split_env(str);
	if (str[ft_strlen(map->key)] != '\0'
		&& str[ft_strlen(map->key) + 1] != '\0')
		map->value = ft_strdup(str + ft_strlen(map->key) + 1);
	else
		map->value = NULL;
	return (map);
}

void	push_env(t_array_list *list, t_string *env)
{
	while (*env != NULL)
	{
		list->push(list, env_to_key_map(*env), sizeof(t_key_map));
		env++;
	}
}
