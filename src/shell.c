//
// Created by Abdelouahad Ait hamd on 11/8/21.
//

#include "minishell.h"

t_bool	shell_parse(t_shell *this)
{
	t_token		*token;
	t_node		*head;

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

	func = (exec_v *) this->exec_pool.find_by_key(this->exec_pool, value);
	if (func == NULL)
		func = &exec_other;
	return (func);
}

t_bool	luanch(t_shell *this, t_node *head)
{
	if (head == NULL)
		return (TRUE);
	if (head->op_type == pipeline)
		pipe(head->p);
	else if (head->word_type == command)
	{
		head->pid = fork();
		if (head->pid == 0)
		{
			find_function(this, head->value);
			exit(1);
		}
		else
		{
			this->last_one = head->pid;
			printf("this is pid {%d} for command %s",
				this->last_one, head->value);
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
			}
		}
	}
	return (luanch(this, head->left) && luanch(this, head->right));
}

t_bool	close_fds(t_shell *this, t_node *head)
{
	if (head == NULL)
		return (TRUE);
	if (head->op_type == pipeline)
	{
		close(head->p[0]);
		close(head->p[1]);
	}
	return (close_fds(this, head->left) && close_fds(this, head->right));
}
