/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_env_ext_next.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 07:29:25 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 07:44:32 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_string	env_ext_next(t_env_ext *this)
{
	int		i;
	char	*env;
	char	*value;

	i = 0;
	if (this->cursor == this->length && this->l_cursor < this->cursor)
		return (strndup(this->cmd + this->l_cursor,
				this->cursor - this->l_cursor));
	if (this->l_cursor > this->cursor || this->cursor >= this->length)
		return (NULL);
	if (this->cmd[this->cursor] == '\"' && !this->q)
	{
		this->dq = !this->dq;
		if (this->l_cursor < this->cursor)
		{
			value = strndup(this->cmd + this->l_cursor,
					this->cursor - this->l_cursor);
			this->l_cursor = this->cursor;
			this->cursor++;
			return (value);
		}
		this->cursor++;
		this->l_cursor = this->cursor;
	}
	if (this->cmd[this->cursor] == '\'' && !this->dq)
	{
		this->q = !this->q;
		this->expand = !this->expand;
		if (this->l_cursor < this->cursor)
		{
			value = strndup(this->cmd + this->l_cursor,
					this->cursor - this->l_cursor);
			this->l_cursor = this->cursor;
			this->cursor++;
			return (value);
		}
		this->cursor++;
		this->l_cursor = this->cursor;
	}
	if (this->cmd[this->cursor] == '$' && (this->cmd[this->cursor +1] == '?'
			|| this->cmd[this->cursor + 1] == '$'))
	{
		this->cursor++;
		if (this->cmd[this->cursor] == '?')
		{
			this->cursor++;
			this->l_cursor = this->cursor;
			return (ft_itoa(this->exit_code % 255));
		}
		else
		{
			this->cursor++;
			this->l_cursor = this->cursor;
			return (strdup("pid not found"));
		}
	}
	if (this->cmd[this->cursor] == '$' && this->expand
		&& !is_digit(this->cmd[this->cursor + 1]))
	{
		i = this->cursor + 1;
		while (this->cmd[i] != '\0' && is_allowed_in_env(this->cmd[i]))
			i++;
		env = strndup(this->cmd + this->cursor + 1, i - (this->cursor + 1));
		value = this->env->find_by_key(*this->env, env);
		if (value == NULL)
			value = strdup("");
		free(env);
		env = ft_strjoin(strndup(this->cmd + this->l_cursor,
					this->cursor - this->l_cursor), value);
		this->cursor = i;
		this->l_cursor = this->cursor;
		return (env);
	}
	else if (this->cmd[this->cursor] == '$')
	{
		value = strndup(this->cmd + this->l_cursor,
				this->cursor - this->l_cursor);
		this->cursor += 2;
		this->l_cursor = this->cursor;
		return (value);
	}
	this->cursor++;
	return (this->next(this));
}
