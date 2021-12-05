#include "minishell.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	arg_is_digit(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

void	cmd_exit(t_shell *this, t_node *head)
{
	t_array_iterator	*iterator;
	char				*exitcode;

	iterator = head->args.iterator(&head->args);
	exitcode = (char *) iterator->next(iterator);
	if (!arg_is_digit(exitcode))
	{
		write(STDERR_FILENO, 255, 3);
		write(STDOUT_FILENO, "exit: ", 7);
		write(STDOUT_FILENO, &exitcode, sizeof(exitcode));
		write(STDOUT_FILENO, " : numeric argument required\n", 30);
		exit(255);
	}
	exit(atoi(exitcode));
}
