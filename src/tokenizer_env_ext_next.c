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

void	new_env_ext(t_env_ext *this, t_array_list *env, char *cmd)
{
	this->env = env;
	this->l_cursor = 0;
	this->cursor = 0;
	this->q = 0;
	this->length = strlen(cmd);
	this->dq = 0;
	this->expand = 1;
	this->cmd = cmd;
	this->has_next = &env_ext_has_next;
	this->next = &env_ext_next;
}
