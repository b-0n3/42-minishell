/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 05:02:15 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/07 05:04:42 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array_list.h"

/*
 ***  @param first_size : array initial size
 ***  @param __sizeofit : sizeof element
 ***  @ param this :  ! please make sure the given pointer  is already
 ***			   allocated or expect a  bus error
 */
void	new_array_list_helper(t_array_list *this)
{
	this->delete_if = &delete_if;
	this->sort = &sort;
	this->free = &array_list_free;
	this->update_at = &update_at;
	this->swap = &ft_swap;
	this->pop = &pop;
	this->pop_i = &pop_index;
	this->swap_i = &swap_i;
	this->filter = &filter_list;
	this->replace_by_key = &map_replace_by_key;
	this->clone = &clone;
	this->push_clone = &push_clone;
	this->find_by_key = &find_by_key;
}

t_array_list	*new_array_list(
		t_array_list *this, size_t first_size, size_t sizeofit)
{
	this->arr = (void *)malloc(first_size * sizeofit);
	if (this->arr == NULL)
		return (NULL);
	this->index = 0;
	this->iterator = &array_list_to_iterator;
	this->length = first_size;
	this->size_of_object = sizeofit;
	this->push = &push;
	this->get = &get;
	this->check_size = &check_size;
	this->check_data_type = &check_data_type;
	this->new_array_double_size = &new_array_double_size;
	this->foreach = &foreach;
	new_array_list_helper(this);
	return (this);
}

t_key_map	*new_key_map(t_string key, void *value)
{
	t_key_map	*map;

	map = malloc(sizeof(t_key_map));
	if (map == NULL)
		return (NULL);
	map->key = key;
	map->value = value;
	return (map);
}

t_bool	map_replace_by_key(t_array_list *this, t_string key,
	void *new_one, void (*del)(void *item))
{
	t_key_map	*old;
	size_t		i;

	i = 0;
	old = NULL;
	while (i < this->index)
	{
		old = (t_key_map *) this->get(this, i);
		if (old != NULL && !ft_strcmp(key, old->key))
		{
			if (new_one == NULL)
				break ;
			if (old->value != NULL)
				del(old->value);
			old->value = new_one;
			return (TRUE);
		}
		old = NULL;
		i++;
	}
	if (old == NULL && i >= this->index)
		this->push(this, new_key_map(strdup(key), new_one), sizeof (t_key_map));
	return (TRUE);
}

void	*find_by_key(t_array_list this, char *key)
{
	t_key_map	*item;
	int			i;

	i = 0;
	if (this.is_map)
	{
		item = (t_key_map *) this.get(&this, i);
		while (item != NULL)
		{
			if (!strcmp(key, (const char *)item->key))
				return (item->value);
			i++;
			item = (t_key_map *) this.get(&this, i);
		}
	}
	return (NULL);
}
