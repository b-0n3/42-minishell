/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 07:02:28 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 08:18:11 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_mood;

t_bool	shell_parse(t_shell *this)
{
	t_token	*token;
	t_node	*head;

	head = NULL;
	while (this->has_next_token(this))
	{
		token = pre_get_next_token(this);
		if (token == NULL && this->has_next_token(this))
		{
			node_free(head);
			head = NULL;
			continue ;
		}
		if (token->type == word)
			head = handle_word(this, token, head);
		else
			head = handle_operator(this, token, head);
	}
	this->head = head;
	return (check_syntax(this, head));
}

void	print_node(t_node *node)
{
	if (node == NULL)
		return ;
	printf("{\n\t\"value\": \"%s\",\n", node->value);
	printf("\n\t\"type\": \"%d\",\n", node->op_type);
	printf("\n\t\"value\": \"%d\",\n", node->word_type);
	printf("\n\t\"type\": \"%d\",\n", node->command_type);
	printf("\n\t\"args\": \n");
	node->args.foreach(&node->args, &print_token);
	printf("}");
	print_node(node->left);
	print_node(node->right);
}

exec_v	*find_function(t_shell *this, t_string value)
{
	exec_v	*func;

	func = (exec_v *)this->exec_pool.find_by_key(this->exec_pool, value);
	if (func == NULL)
		func = &exec_other;
	return (func);
}

void	exec_heredoc(t_shell *this, t_node *head)
{
	char	*buffer;

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

t_bool	launch(t_shell *this, t_node *head)
{
	t_node	*tmp;

	if (head == NULL)
		return (TRUE);
	if (head->op_type == pipeline)
		pipe(head->p);
	if (head->op_type == heredoc)
	{
		head->pid = fork();
		if (head->pid == 0)
		{
			exec_heredoc(this, head);
		}
		else
		{
			waitpid(head->pid, NULL, 0);
			head->output_file->open(head->output_file, O_RDONLY, -1);
		}
	}
	else if (head->word_type == command)
	{
		head->pid = fork();
		if (head->pid == 0)
		{
			find_function(this, head->value)(this, head);
			exit(1);
		}
		else
		{
			this->last_one = head->pid;
			if (head->parent != NULL)
			{
				if (head->parent->op_type == pipeline)
				{
					if (!head->isleft)
					{
						close(head->parent->p[1]);
						close(head->parent->p[0]);
					}
				}
				tmp = head->parent;
				while (tmp->parent != NULL
					&& tmp->parent->need_a_file(tmp->parent))
					tmp = tmp->parent;
				if (tmp->need_a_file(tmp)
					&& tmp->parent != NULL && tmp->parent->op_type == pipeline)
				{
					fprintf(stderr, "close in  %s \n", tmp->value);
					close(tmp->parent->p[1]);
					close(tmp->parent->p[0]);
				}
			}
		}
	}
	return (launch(this, head->left) && launch(this, head->right));
}
