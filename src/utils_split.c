/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 04:07:41 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/07 04:07:53 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_array_iterator	*split(t_string cmd, char ch)
{
	t_array_list	*list;

	if (cmd == NULL)
		return (NULL);
	list = malloc(sizeof(t_array_list));
	new_array_list(list, 2, sizeof(char *));
	if (list == NULL)
		return (NULL);
	split_it(list, cmd, ch);
	return (list->iterator(list));
}
