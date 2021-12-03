#include "minishell.h"

t_node	*handle_operator(t_shell *this, t_token *token, t_node *head)
{
	t_node	*operator;

	if (token == NULL)
		return (head);
	operator = token->to_node(token);
	if (operator->op_type == pipeline)
	{
		if (head != NULL)
		{
			operator->left = head;
			head->isleft = TRUE;
			head->parent = operator;
			head = operator;
		}
		else
			head = operator;
	}
	else if (head != NULL)
	{
		if (head->op_type == pipeline)
		{
			if (head->right != NULL)
			{
				operator->left = head->right;
				head->right->isleft = TRUE;
				head->right->parent = operator;
				head->right = operator;
				operator->isleft = FALSE;
			}
			else
			{
				operator->isleft = FALSE;
				head->right = operator;
				head->right->parent = head;
			}
		}
		else
		{
			operator->left = head;
			head->isleft = TRUE;
			head->parent = operator;
			head = operator;
		}
	}
	else
		head = operator;
	return (head);
}
