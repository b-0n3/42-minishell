/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 07:26:11 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 07:28:02 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	is_allowed_in_env(char ch)
{
	return ((ch >= 'a' && ch <= 'z')
		|| (ch >= 'A' && ch <= 'Z')
		|| (ch >= '0' && ch <= '9')
		|| (ch == '_'));
}

t_string	cut_word(t_env_ext *this)
{
	return (strndup(this->cmd + this->l_cursor,
			this->cursor - this->l_cursor));
}

t_bool	is_digit(char ch)
{
	return (ch >= '0' && ch <= '9');
}

t_string	ft_strjoin(t_string s1, t_string s2)
{
	t_string	result;

	if (s1 == NULL)
		return (s2);
	if (s2 == NULL)
		return (s1);
	result = calloc(strlen(s1) + strlen(s2) + 1, 1);
	memcpy(result, s1, strlen(s1));
	memcpy(result + strlen(s1), s2, strlen(s2));
	free(s1);
	return (result);
}

t_bool	env_ext_has_next(t_env_ext *this)
{
	return (this->cursor < this->length);
}
