#include "minishell.h"

t_token	*get_op(t_shell *this)
{
	t_token	*token;

	if (this->quot || this->dqout)
		return (NULL);
	token = check_pipe(this);
	if (token == NULL)
	{
		token = check_great(this);
		if (token == NULL)
			token = check_less(this);
	}
	return (token);
}

t_token	*cut_token(t_shell *this)
{
	t_token	*token;

	if (this->quot || this->dqout)
		return (NULL);
	token = get_op(this);
	if (token == NULL)
		token = get_word(this);
	return (token);
}
//// cat | hell
////		   ^

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
