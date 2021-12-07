/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 04:23:57 by aait-ham          #+#    #+#             */
/*   Updated: 2021/12/07 04:43:06 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	result = ft_calloc(ft_strlen(s1) + ft_strlen(s2) + 1, 1);
	ft_memcpy(result, s1, ft_strlen(s1));
	ft_memcpy(result + ft_strlen(s1), s2, ft_strlen(s2));
	free(s1);
	return (result);
}

t_bool	env_ext_has_next(t_env_ext *this)
{
	return (this->cursor < this->length);
}

t_token	*cut_token_op(t_shell *this)
{
	t_token	*token;

	if (this->l_cursor < this->cursor)
			token = get_word(this);
	else
			token = get_op(this);
	return (token);
}

t_token	*shell_get_next_token(t_shell *this)
{
	if (this->cursor <= this->l_cursor
		&& this->cursor >= this->command_len)
		return (NULL);
	if (this->cursor > this->command_len
		&& this->unclosed(this))
		return (NULL);
	if (this->cursor > this->command_len)
		return (NULL);
	if (this->cursor == this->command_len)
		return (cut_token(this));
	if (check_unclosed(this))
		return (this->get_next_token(this));
	if ((this->commmand[this->cursor] == '\'' && this->quot)
		|| (this->commmand[this->cursor] == '\"' && this->dqout))
		return (get_word(this));
	if (check_op(this->commmand[this->cursor])
		&& !this->unclosed(this))
		return (cut_token_op(this));
	if (this->commmand[this->cursor] == ' ' && !this->unclosed(this)
		&& this->cursor > this->l_cursor)
		return (get_word(this));
	this->cursor++;
	return (this->get_next_token(this));
}
