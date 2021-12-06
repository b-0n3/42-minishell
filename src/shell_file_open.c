/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_file_open.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 06:34:33 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 06:40:15 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_mood;

t_bool	file_open(t_file *this, int mode, int perms)
{
	if (perms > 0)
		this->fd = open(this->uri, mode, perms);
	else
		this->fd = open(this->uri, mode);
	this->exception = this->fd < 0;
	return (this->exception);
}

void	split_it(t_array_list *this, t_string cmd, char ch)
{
	int	dq;
	int	q;
	int	cursor;
	int	l_cursor;

	cursor = 0;
	l_cursor = 0;
	q = 0;
	dq = 0;
	while (cmd[cursor] != '\0')
	{
//		if (cmd[cursor] == '\"' && !q)
//		   dq = !dq;
//		if (cmd[cursor] == '\'' && !dq)
//			q = !q;
		if (cmd[cursor] == ch)
		{
			this->push(this, strndup(cmd + l_cursor,
					cursor - l_cursor), sizeof(char *));
			while (cmd[cursor] == ch)
				cursor++;
			l_cursor = cursor;
		}
		cursor++;
	}
	if (l_cursor < cursor)
		this->push(this, strndup(cmd + l_cursor,
				cursor - l_cursor), sizeof(char *));
}

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

void	*str_to_token(void *str)
{
	return (new_token((t_string)str, word));
}

t_node	*token_to_node(t_token *this)
{
	t_node				*node;
	t_array_iterator	*iter;

	node = new_node();
	if (node == NULL)
		return (NULL);
	node->value = strdup(this->value);
	if (this->type == word)
	{
		node->word_type = command;
		if (this->start_with != '\"' && this->start_with != '\'')
		{
			iter = split(this->value, ' ');
			if (iter->list->index > 1)
			{
				free(node->value);
				node->value = (char *)iter->next(iter);
				while (iter->has_next(iter))
					node->args.push(&node->args,
						iter->do_on_next(iter, &str_to_token), sizeof(t_token));
			}
			iter->free(iter, &free);
		}
	}
	else
		node->op_type = find_operation_type(this->value);
	return (node);
}
