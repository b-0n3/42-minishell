/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_list_clone.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/11 19:08:17 by aait-ham          #+#    #+#             */
/*   Updated: 2021/07/11 19:08:37 by aait-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array_list.h"

t_array_list	*clone(struct s_array_list *this,
		t_bool is_not_primitive, void *(*__clone)(void *item))
{
	t_array_list	*clone;

	clone = malloc(sizeof(t_array_list));
	if (this != NULL)
	{
		new_array_list(clone, this->length, this->size_of_object);
		clone->push_clone(clone, *this, is_not_primitive, __clone);
	}
	return (clone);
}

t_bool	push_clone(t_array_list *this, t_array_list original,
		t_bool is_not_primitive, void *(*__clone)(void *item)){
	void	*item;
	void	*cln;

	if (this == NULL
		|| this->size_of_object != original.size_of_object
		|| (is_not_primitive && __clone == NULL))
		return (FALSE);
	item = original.pop(&original);
	while (item != NULL)
	{
		if (is_not_primitive)
			cln = __clone(item);
		else
		{
			cln = malloc(this->size_of_object);
			cln = memcpy(cln, item, this->size_of_object);
		}
		this->push(this, cln, this->size_of_object);
		item = original.pop(&original);
	}
	return (TRUE);
}
