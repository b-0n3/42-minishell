/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_extracter_.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 04:17:10 by aait-ham          #+#    #+#             */
/*   Updated: 2021/12/07 04:34:40 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_string	env_ext_next(t_env_ext *this)
{
	char	*value;

	if (this->cursor == this->length
		&& this->l_cursor < this->cursor)
		return (cut_w(this));
	if (this->l_cursor > this->cursor
		|| this->cursor >= this->length)
		return (NULL);
	value = extract_after_q(this);
	if (value != NULL)
		return (value);
	if (this->cmd[this->cursor] == '$'
		&& (this->cmd[this->cursor +1] == '?'
			|| this->cmd[this->cursor + 1] == '$'))
		extract_exit_code(this);
	if (this->cmd[this->cursor] == '$'
		&& this->expand && !ft_is_digit(this->cmd[this->cursor + 1]))
		return (extract_env(this));
	else if (this->cmd[this->cursor] == '$')
		return (extract_env_(this));
	this->cursor++;
	return (this->next(this));
}

t_bool	is_allowed_in_env(char ch)
{
	return ((ch >= 'a' && ch <= 'z')
		|| (ch >= 'A' && ch <= 'Z')
		|| (ch >= '0' && ch <= '9')
		|| (ch == '_'));
}

void	new_env_ext(t_env_ext *this, t_array_list *env,
		char *cmd, int exit_code)
{
	this->env = env;
	this->l_cursor = 0;
	this->cursor = 0;
	this->exit_code = exit_code;
	this->q = 0;
	this->length = strlen(cmd);
	this->dq = 0;
	this->expand = 1;
	this->cmd = cmd;
	this->has_next = &env_ext_has_next;
	this->next = &env_ext_next;
}

t_token	*token_expand_env(t_token *this, int exit_code, t_array_list env)
{
	t_env_ext	env_ext;
	t_string	t;
	t_string	tmp;

	new_env_ext(&env_ext, &env, this->value, exit_code);
	t = ft_strdup("");
	while (env_ext.has_next(&env_ext))
	{
		tmp = env_ext_next(&env_ext);
		if (tmp != NULL)
		{
			t = ft_strjoin(t, tmp);
			free(tmp);
		}
	}
	free(this->value);
	this->value = t;
	return (this);
}
