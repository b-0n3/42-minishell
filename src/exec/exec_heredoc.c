/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 03:32:43 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/07 03:33:23 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_heredoc(t_shell *this, t_node *head)
{
	char	*buffer;

	this = NULL;
	buffer = readline(">");
	while (buffer != NULL && strcmp(buffer, head->eof) != 0)
	{
		write(head->output_file->fd, buffer, strlen(buffer));
		write(head->output_file->fd, "\n", 1);
		free(buffer);
		buffer = readline(">");
	}
	if (buffer != NULL)
		free(buffer);
	close(head->output_file->fd);
	exit(0);
}

t_bool	exec_all_heredocs(t_shell *this, t_node *head)
{
	if (head == NULL)
		return (TRUE);
	if (head->op_type == heredoc)
	{
		head->pid = fork();
		if (head->pid == 0)
		{
			signal(SIGQUIT, &child_quit);
			signal(SIGINT, &child_quit);
			exec_heredoc(this, head);
		}
		else
		{
			waitpid(head->pid, NULL, 0);
			head->output_file->open(head->output_file, O_RDONLY, -1);
		}
	}
	return (exec_all_heredocs(this, head->left)
		&& exec_all_heredocs(this, head->right));
}
