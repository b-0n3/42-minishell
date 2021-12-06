/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 09:32:02 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 10:59:47 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_nl(void *item)
{
	if (strcmp((char *)item, "-n"))
		return (1);
	else
		return (0);
}

void	cmd_echo(t_shell *this, t_node *head)
{
	t_array_iterator	*iterator;
	int					newline;

	newline = 0;
	if (head->args.index > 0)
		iterator = (char *)((t_token *)head->args.get(&head->args, 0))->value;
	while (iterator->has_next(iterator) && newline == 0)
		newline += (int) iterator->do_on_next(iterator, &check_nl);
	write(STDOUT_FILENO, &iterator, strlen((char *)iterator));
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
}
