/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 10:06:12 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 10:59:39 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

	if (head->args.index > 0)
		iterator = (char *)((t_token *)head->args.get(&head->args, 0))->value;
	else
		iterator = NULL;
	exitcode = (char *) iterator->next(iterator);
	if (!arg_is_digit(exitcode))
	{
		write(stderr, 255, 3);
		write(STDOUT_FILENO, "exit: ", 7);
		write(STDOUT_FILENO, &exitcode, sizeof(exitcode));
		write(STDOUT_FILENO, " : numeric argument required\n", 30);
		exit(255);
	}
	exit(atoi(exitcode));
}
