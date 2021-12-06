/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_executes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 07:11:08 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 11:10:07 by am-khant         ###   ########.fr       */
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
	int exi;

	if (head == NULL)
		return (TRUE);
	if (head->word_type == command && this->last_one != head->pid)
		waitpid(head->pid, &exi, 0);
	return (wait_for_all(this, head->left) && wait_for_all(this, head->right));
}

t_bool	is_built_in(t_string n)
{
	return (!strcmp(n, "cd") || !strcmp(n, "pwd"));
}

void	shell_execute(t_shell *this)
{
	g_mood = 1;
	if ((this->head->parent == NULL
			|| this->head->parent->need_a_file(this->head->parent))
		&& is_built_in(this->head->value))
		find_function(this, this->head->value)(this, this->head);
	else
	{
		launch(this, this->head);
		wait_for_all(this, this->head);
		waitpid(this->last_one, &this->exit_code, 0);
	}
	close_fds(this, this->head);
	g_mood = 0;
}

void	*map_to_string(void *item)
{
	t_key_map	*m;
	t_string	tmp;

	m = (t_key_map *) item;
	tmp = strdup(m->key);
	tmp = ft_strjoin(tmp, "=");
	return (ft_strjoin(tmp, (char *)m->value));
}
