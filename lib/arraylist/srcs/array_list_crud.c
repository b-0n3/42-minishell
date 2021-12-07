/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_list_crud.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 19:36:12 by aait-ham          #+#    #+#             */
/*   Updated: 2021/07/11 19:08:30 by aait-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array_list.h"

t_bool	update_at(t_array_list *this, void *value, size_t index)
{
	if (index < this->index && value != NULL)
	{
		this->arr[index] = value;
		return (TRUE);
	}
	return (FALSE);
}

void	*get(t_array_list *this, size_t index)
{
	if (this->index > index && index >= 0)
		return (this->arr[index]);
	return (NULL);
}

void	array_list_free(t_array_list *this, void (*fe)(void *item))
{
	if (this != NULL && fe != NULL)
		this->foreach(this, fe);
	this->index = 0;
	free(this->arr);
	this->arr = NULL;
}
