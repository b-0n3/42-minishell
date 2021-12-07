/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_checks.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 04:23:45 by aait-ham          #+#    #+#             */
/*   Updated: 2021/12/07 04:37:59 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*check_pipe(t_shell *this)
{
	if (this->commmand[this->cursor] == _pipe)
	{
		this->cursor++;
		while (this->commmand[this->cursor] == ' ')
			this->cursor++;
		this->l_cursor = this->cursor;
		return (new_token(ft_strdup("|"), op));
	}
	return (NULL);
}

t_token	*check_less(t_shell *this)
{
	t_token	*token;

	token = NULL;
	if (this->commmand[this->cursor] == less)
	{
		this->cursor++;
		this->l_cursor = this->cursor;
		token = new_token(ft_strdup("<"), op);
		if (this->commmand[this->cursor] == less)
		{
			free(token->value);
			token->value = ft_strdup("<<");
			this->cursor++;
			this->l_cursor = this->cursor;
		}
		while (this->commmand[this->cursor] == ' ')
			this->cursor++;
		this->l_cursor = this->cursor;
	}
	return (token);
}

t_token	*check_great(t_shell *this)
{
	t_token	*token;

	token = NULL;
	if (this->commmand[this->cursor] == great)
	{
		this->cursor++;
		this->l_cursor = this->cursor;
		token = new_token(ft_strdup(">"), op);
		if (this->commmand[this->cursor] == great)
		{
			free(token->value);
			token->value = ft_strdup(">>");
			this->cursor++;
		}
		while (this->commmand[this->cursor] == ' ')
			this->cursor++;
		this->l_cursor = this->cursor;
	}
	return (token);
}

t_bool	check_op(char ch)
{
	return (ch == less || ch == great || ch == _pipe);
}

t_bool	check_unclosed(t_shell *this)
{
	if (!this->has_next_token(this))
		return (FALSE);
	if (this->commmand[this->cursor] == '\"' && !this->quot)
	{
		this->dqout = !this->dqout;
		this->cursor++;
		return (TRUE);
	}
	if (this->commmand[this->cursor] == '\'' && !this->dqout)
	{
		this->quot = !this->quot;
		this->cursor++;
		return (TRUE);
	}
	return (FALSE);
}
