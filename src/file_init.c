/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 03:15:39 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/07 03:16:22 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_file	*token_to_file(t_token *this)
{
	if (this == NULL)
		return (NULL);
	return (new_file(this->value));
}

t_file	*new_file(t_string uri)
{
	t_file	*file;

	file = malloc(sizeof (t_file));
	file->uri = ft_strdup(uri);
	file->exception = 0;
	file->open = &file_open;
	file->fd = -1;
	return (file);
}

t_bool	file_open(t_file *this, int mode, int perms)
{
	if (perms > 0)
		this->fd = open(this->uri, mode, perms);
	else
		this->fd = open(this->uri, mode);
	this->exception = this->fd < 0;
	return (this->exception);
}
