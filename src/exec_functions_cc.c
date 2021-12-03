#include "minishell.h"

void	exec_env(t_shell *this, t_node *node)
{
	printf("env command \n");
}

void	exec_exit(t_shell *this, t_node *node)
{
	printf("exit command \n");
}

t_bool	end_with(t_string s, char c)
{
	int	i;

	i = strlen(s);
	while (i > 0 && s[i] == ' ')
		i--;
	return (s[i] == c);
}
