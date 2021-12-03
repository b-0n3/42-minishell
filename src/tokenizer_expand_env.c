#include "minishell.h"

t_token	*token_expand_env(t_token *this, t_array_list env)
{
	t_env_ext	env_ext;
	t_string	t;
	t_string	tmp;

	new_env_ext(&env_ext, &env, this->value);
	t = strdup("");
	while (env_ext.has_next(&env_ext))
	{
		tmp = env_ext_next(&env_ext);
		if (tmp != NULL)
			t = ft_strjoin(t, tmp);
	}
	free(this->value);
	this->value = t;
	return (this);
}
// cat herll | dsf  > dsf  <<ff
//   ^		 ^
/*
 * todo : fix space after quotes
   * @return a token if found or null in case of error or find unclosed quot
 * */

t_token	*shell_get_next_token(t_shell *this)
{
	t_token	*token;

	if (this->cursor <= this->l_cursor && this->cursor >= this->command_len)
		return (NULL);
	if (this->cursor > this->command_len && this->unclosed(this))
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
	if (check_op(this->commmand[this->cursor]) && !this->unclosed(this))
	{
		if (this->l_cursor < this->cursor)
			token = get_word(this);
		else
			token = get_op(this);
		return (token);
	}
	if (this->commmand[this->cursor] == ' ' && !this->unclosed(this)
		&& this->cursor > this->l_cursor)
		return (get_word(this));
	this->cursor++;
	return (this->get_next_token(this));
}
