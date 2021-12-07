/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_extracter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 04:17:05 by aait-ham          #+#    #+#             */
/*   Updated: 2021/12/07 04:30:31 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_string	extract_env(t_env_ext *this)
{
	int		i;
	char	*env;
	char	*value;

	i = this->cursor + 1;
	while (this->cmd[i] != '\0' && is_allowed_in_env(this->cmd[i]))
		i++;
	env = strndup(this->cmd + this->cursor + 1,
			i - (this->cursor + 1));
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

t_string	extract_exit_code(t_env_ext *this)
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
		return (ft_strdup("pid not found"));
	}
}

t_string	extract_string(t_env_ext *this)
{
	t_string	value;

	if (this->l_cursor < this->cursor)
	{
		value = ft_strndup(this->cmd + this->l_cursor,
				this->cursor - this->l_cursor);
		this->cursor++;
		this->l_cursor = this->cursor;
		return (value);
	}
	return (NULL);
}

t_string	extract_after_q(t_env_ext *this)
{
	t_string	value;

	if (this->cmd[this->cursor] == '\"' && !this->q)
	{
		this->dq = !this->dq;
		value = extract_string(this);
		if (value != NULL)
			return (value);
		this->cursor++;
		this->l_cursor = this->cursor;
	}
	if (this->cmd[this->cursor] == '\'' && !this->dq)
	{
		this->q = !this->q;
		this->expand = !this->expand;
		value = extract_string(this);
		if (value != NULL)
			return (value);
		this->cursor++;
		this->l_cursor = this->cursor;
	}
	return (NULL);
}

t_string	extract_env_(t_env_ext *this)
{
	t_string	value;

	value = ft_strndup(this->cmd + this->l_cursor,
			this->cursor - this->l_cursor);
	this->cursor += 2;
	this->l_cursor = this->cursor;
	return (value);
}
