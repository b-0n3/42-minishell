#include "minishell.h"

t_token	*new_token(t_string value, t_token_type type)
{
	t_token	*token;
	int		i;

	i = 0;
	token = malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	while (value[i] == ' ')
		i++;
	token->value = value;
	token->type = type;
	token->start_with = value[i];
	token->to_node = &token_to_node;
	token->free = &token_free;
	token->expand = &token_expand_env;
	token->to_file = &token_to_file;
	return (token);
}

t_token	*check_pipe(t_shell *this)
{
	if (this->commmand[this->cursor] == _pipe)
	{
		this->cursor++;
		while (this->commmand[this->cursor] == ' ')
			this->cursor++;
		this->l_cursor = this->cursor;
		return (new_token(strdup("|"), op));
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
		token = new_token(strdup("<"), op);
		if (this->commmand[this->cursor] == less)
		{
			free(token->value);
			token->value = strdup("<<");
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
		token = new_token(strdup(">"), op);
		if (this->commmand[this->cursor] == great)
		{
			free(token->value);
			token->value = strdup(">>");
			this->cursor++;
		}
		while (this->commmand[this->cursor] == ' ')
			this->cursor++;
		this->l_cursor = this->cursor;
	}
	return (token);
}

t_token	*get_word(t_shell *this)
{
	t_token	*token;

	if (this->quot || this->dqout)
		return (NULL);
	if (this->cursor <= this->l_cursor)
		return (NULL);
	token = new_token(strndup(this->commmand + this->l_cursor,
				this->cursor - this->l_cursor), word);
	while (this->commmand[this->cursor] == ' ')
		this->cursor++;
	this->l_cursor = this->cursor;
	return (token);
}
