/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_list.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 19:37:51 by aait-ham          #+#    #+#             */
/*   Updated: 2021/07/11 19:08:06 by aait-ham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARRAY_LIST_H
# define ARRAY_LIST_H
# include <string.h>
# include <stdio.h>
# include <stdlib.h>


typedef int	t_bool;

# define TRUE 1
# define FALSE 0
typedef char * t_string;
typedef struct s_array_list t_array_list;

/**
 * @To_string the f function must write item like this "key":"value"
 */
typedef struct s_key_map{
    t_string key;
    void *value;
    void (*to_string)(struct s_key_map *this, void (*f)(void *item));
}   t_key_map;


// Todo: implement iterator  and create a repo for this not copying everytime 
typedef struct s_array_iterator{
		size_t next_index;
		t_array_list *list;
		t_bool	(*has_next)(struct s_array_iterator *this);
		void (*reset)(struct s_array_iterator *);
		void *(*next)(struct s_array_iterator *this);
		void *(*do_on_next)(struct s_array_iterator *this, void *(*f)(void *item));
        void *(*do_on_next_p)(struct s_array_iterator *this, void *(*f)(void *, void *), void *p_item);
        void (*free)(struct s_array_iterator *this, void (*free)(void *));

}			t_array_iterator;

 struct s_array_list{
	void					**arr;
	size_t					index;
	size_t					length;
    t_bool                  is_map;
	size_t					size_of_object;

	t_array_iterator *(*iterator)(struct s_array_list *this);

	t_bool					(*push)(struct s_array_list *this, void *value,
			size_t	size_of_item);
	t_bool					(*push_clone)(struct s_array_list *this,
		struct s_array_list original,
			t_bool is_not_primitive, void *(*__clone)(void *item));
	void					*(*get)(struct s_array_list *this, size_t index);
	t_bool					(*check_size)(struct s_array_list *this);
	t_bool					(*check_data_type)(struct s_array_list *this,
			size_t size_of_item);
	t_bool					(*new_array_double_size)(struct s_array_list *this);
	void					(*foreach)(struct s_array_list *this,
			void (*f)(void *item));
	void					(*delete_if)(struct s_array_list *this,
			t_bool (*cond)(void *item), void (*fe)(void *item));
	void					(*sort)(struct s_array_list *this,
			int					(*cond)(void *item1, void *item2),
			int l, int r);
	void					(*free)(struct s_array_list *this,
			void (*fe)(void *item));
	t_bool					(*update_at)(struct s_array_list *this,
			void *value, size_t index);
	void					(*swap_i)(struct s_array_list *this,
			size_t i1, size_t i2);
	void					(*swap)(void **a, void **b);
	void					*(*pop)(struct s_array_list *this);
	void					*(*pop_i)(struct s_array_list *this, size_t index);
	struct	s_array_list	*(*filter)(struct s_array_list	list,
			size_t	size,
			t_bool	(*cond)(void *item), void *(*map)(void *item));
	struct	s_array_list	*(*clone)(struct s_array_list *this,
			t_bool is_not_primitive, void *(*__clone)(void *item));
    void *(*find_by_key)(struct s_array_list this, t_string  key);
}	;

t_array_list	*new_array_list(t_array_list *this,
					size_t first_size, size_t sizeofit);
t_bool			push(t_array_list *this, void *value,
					size_t size_of_item);
void			*get(t_array_list *this, size_t index);
t_bool			check_size(t_array_list *this);
t_bool			check_data_type(t_array_list *this,
					size_t size_of_item);
t_bool			new_array_double_size(t_array_list *this);
void			foreach(t_array_list *this, void (*f)(void *item));
void			delete_if(t_array_list *this,
					t_bool(*cond)(void *item), void (*fe)(void *item));
void			sort(t_array_list *this,
					int (*cond)(void *item1, void *item2), int l, int r);
void			array_list_free(t_array_list *this,
					void (*fe)(void *item));
void			ft_swap(void **a, void **b);
t_bool			update_at(struct s_array_list *this,
					void *value, size_t index);
void			*pop(t_array_list *this);
void			*pop_index(t_array_list *this, size_t index);
t_array_list	*filter_list(t_array_list list, size_t size,
					t_bool (*cond)(void *item), void *(*map)(void *item));
void			swap_i(t_array_list *this, size_t i1, size_t i2);
t_array_list	*clone(t_array_list *this, t_bool is_not_primitive,
					void *(*__clone)(void *item));
t_bool			push_clone(t_array_list *this, t_array_list original,
					t_bool is_not_primitive, void *(*__clone)(void *item));
t_bool	iterator_has_next(t_array_iterator *this);
void iterator_reset(t_array_iterator *this);
t_array_iterator *array_list_to_iterator(t_array_list *this);
t_array_iterator *new_iterator(t_array_list *list);
void *iterator_next(t_array_iterator *this);
void *iterator_do_on_next(t_array_iterator *this, void *(*f)(void *));
void iterator_free(t_array_iterator *this, void (*free)(void *));
void *find_by_key(t_array_list  this, t_string key);
void *iterator_do_on_next_p(t_array_iterator *this, void *(*f)(void *, void *), void *p_item);

#endif
