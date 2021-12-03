#include "minishell.h"

t_bool	wait_for_all(t_shell *this, t_node *head)
{
	int	exi;

	if (head == NULL)
		return (TRUE);
	if (head->word_type == command && this->last_one != head->pid)
	{
		waitpid(head->pid, &exi, 0);
	}
	return (wait_for_all(this, head->left) && wait_for_all(this, head->right));
}

void	shell_execute(t_shell *this)
{
	luanch(this, this->head);
	wait_for_all(this, this->head);
	waitpid(this->last_one, &this->exit_code, 0);
	close_fds(this, this->head);
}

void	shell_loop(t_shell *this)
{
	t_string	line;

	line = readline("IM->B0N3$>");
	while (line != NULL && strcmp(line, "exit") != 0)
	{
		this->init(this, line);
		if (this->parse(this))
			this->execute(this);
		else
			perror(this->parsing_error);
		line = readline("IM->B0N3$>");
	}
}

void	shell_free(t_shell *this)
{
	return ;
}
