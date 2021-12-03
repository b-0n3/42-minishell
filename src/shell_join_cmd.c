#include "minishell.h"

t_bool	has_next_token(t_shell *this)
{
	return (this->cursor < this->command_len);
}

char	*join_command(t_string origin, t_string next)
{
	size_t		len;
	t_string	result;

	if (next == NULL || origin == NULL)
		return (NULL);
	len = strlen(origin) + strlen(next);
	result = malloc(len + 3);
	if (result == NULL)
		return (NULL);
	strcpy(result, origin);
	strcpy(result + strlen(origin), "\n");
	strcpy(result + strlen(result), next);
	return (result);
}

t_bool	shell_quot_unclosed(t_shell *this)
{
	return (this->quot || this->dqout);
}

void	print_token(void *item)
{
	t_token	*m;

	if (item != NULL)
	{
		m = (t_token *) item;
		printf("\t{\n\t\"type\":\"%s\",\n \t\"value\":\"%s\"\n}\n",
			m->type == op? "OP": "WORD", m->value);
	}
}

t_token	*pre_get_next_token(t_shell *this)
{
	t_token		*token;
	char		*next;

	token = NULL;
	if (this->unclosed(this))
	{
		next = join_command(this->commmand, readline(">"));
		if (next == NULL)
			return (NULL);
		this->init(this, next);
		return (pre_get_next_token(this));
	}
	token = this->get_next_token(this);
	return (token);
}
