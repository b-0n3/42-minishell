/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator_suite.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 07:59:58 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 08:00:43 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array_list.h"

void	*iterator_do_on_next_p(t_array_iterator *this,
		void *(*f)(void *, void *), void *p_item)
{
	void	*ptr;

	ptr = NULL;
	if (this != NULL)
	{
		ptr = this->next(this);
		if (ptr == NULL)
			return (NULL);
		ptr = f(ptr, p_item);
	//this->list->update_at(this->list, ptr, this->next_index - 1);
	}
	return (ptr);
}

t_bool	iterator_has_next(t_array_iterator *this)
{
	if (this == NULL)
		return (FALSE);
	return (this->next_index < this->list->index);
}

void	iterator_free(t_array_iterator *this, void (*f)(void *))
{
	if (this != NULL)
	{
		if (f != NULL)
			this->list->free(this->list, f);
		free(this);
	}
}
