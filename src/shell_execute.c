/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_execute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 05:58:32 by aait-ham          #+#    #+#             */
/*   Updated: 2021/12/07 06:03:16 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_mood;

t_bool	close_fds(t_shell *this, t_node *head)
{
	if (head == NULL)
		return (TRUE);
	if (head->op_type == pipeline)
	{
		close(head->p[0]);
		close(head->p[1]);
	}
	else if (head->need_a_file(head))
	{
		if (head->output_file != NULL && head->output_file->exception == 0)
			close(head->output_file->fd);
		if (head->input_file != NULL && head->input_file->exception == 0)
			close(head->input_file->fd);
	}
	return (close_fds(this, head->left) && close_fds(this, head->right));
}

t_bool	wait_for_all(t_shell *this, t_node *head)
{
	int	exi;

	if (head == NULL)
		return (TRUE);
	if (head->word_type == command && this->last_one != head->pid)
		waitpid(head->pid, &exi, 0);
	return (wait_for_all(this, head->left) && wait_for_all(this, head->right));
}

t_bool	is_built_in(t_string n)
{
	return (!ft_strcmp(n, "cd") || !ft_strcmp(n, "pwd") || !ft_strcmp(n, "echo")
		|| !ft_strcmp(n, "export") || !ft_strcmp(n, "env")
		|| !ft_strcmp(n, "unset")
		|| !ft_strcmp(n, "exit"));
}

t_exec_v	*find_function(t_shell  *this, t_string value)
{
	t_exec_v	*func;

	func = (t_exec_v *)this->exec_pool.find_by_key(this->exec_pool, value);
	if (func ==  NULL)
		func = &exec_other;
	return (func);
}

void	shell_execute(t_shell *this)
{
	t_exec_v	*fun;

	g_mood = 1;
	if (this->head == NULL)
		return ;
	exec_all_heredocs(this, this->head);
	if ((this->head->parent == NULL
			|| this->head->parent->need_a_file(this->head->parent))
		&& is_built_in(this->head->value))
	{
		g_mood = 2;
		fun = find_function(this, this->head->value);
		fun(this, this->head);
		g_mood = 1;
	}
	else
	{
		launch(this, this->head);
		wait_for_all(this, this->head);
		waitpid(this->last_one, &this->exit_code, 0);
	}
	close_fds(this, this->head);
	g_mood = 0;
}
