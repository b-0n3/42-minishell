/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_list_pull.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 19:37:04 by aait-ham          #+#    #+#             */
/*   Updated: 2021/07/11 19:09:31 by aait-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array_list.h"

/*
 *** @author b0n3@1337
 ***	this{ 1,1,2,3,4,5,5,6,7}
 *** i      /
 *** [1,1,2,4,5,5,6,7]
 *** this function  return item with the index
 *** i and delete it from the array List
 *** @param this (pointer to this arrayList ) / index item index
 *** @return this[i]
 */

void	*pop_index(t_array_list *this, size_t index)
{
	void	*ptr;
	size_t	i;

	ptr = NULL;
	if (this->index > index)
	{
		ptr = this->get(this, index);
		i = index;
		while (i++ < this->index)
			this->update_at(this, this->get(this, i + 1), i);
		this->index--;
	}
	return (ptr);
}

void	swap_i(t_array_list *this, size_t i1, size_t i2)
{
	if (this != NULL && i1 != i2)
		if (i1 < this->index && i2 < this->index)
			this->swap(&this->arr[i1], &this->arr[i2]);
}
