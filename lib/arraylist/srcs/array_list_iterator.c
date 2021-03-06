/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_list_iterator.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 07:54:47 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 07:54:48 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array_list.h"

t_array_iterator	*array_list_to_iterator(t_array_list *this)
{
	t_array_iterator	*iterator;

	iterator = NULL;
	if (this != NULL)
	{
		iterator = new_iterator(this);
	}
	return (iterator);
}
