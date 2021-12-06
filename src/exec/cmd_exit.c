#include "minishell.h"

extern int mood;

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
	char				*exitcode;
    t_token            *token;
    int                 e_c;
    token = head->args.get(&head->args, 0);
    if (token != NULL && token->value != NULL)
        exitcode = strdup(token->value);
    else
        exitcode = NULL;

	if (exitcode != NULL && !arg_is_digit(exitcode))
	{
		write(STDOUT_FILENO, "exit: ", 7);
		write(STDOUT_FILENO, &exitcode, sizeof(exitcode));
		write(STDOUT_FILENO, " : numeric argument required\n", 30);
        if (mood == 2) {
             free(exitcode);
            this->free(this);
        }
            exit(255);
	}
    if (mood == 2)
        write(STDOUT_FILENO, "exit\n",5);
    if (mood == 2)
        this->free(this);
    if (exitcode!= NULL) {
        e_c = atoi(exitcode);
        free(exitcode);
        exit(e_c);
    }
    exit(0);
}
