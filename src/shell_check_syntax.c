#include "minishell.h"

t_bool	check_syntax(t_shell *this, t_node *pointer)
{
	t_node	*tmp;

	tmp = pointer;
	if (pointer == NULL)
		return (TRUE);
	if (this != NULL && this->parsing_error != NULL)
		return (FALSE);
	if (pointer->op_type == pipeline)
	{
		if (pointer->left == NULL || pointer->right == NULL)
		{
			this->parsing_error = strdup("unexpected token |");
			return (FALSE);
		}
		if (pointer->right != NULL && pointer->right->word_type == w_t_none)
		{
			pointer = pointer->right;
			while (pointer != NULL && pointer->op_type != o_t_none)
				pointer = pointer->left;
			if (pointer == NULL)
			{
				this->parsing_error = strdup("unexpected token |");
				return (FALSE);
			}
		}
		if (pointer->left != NULL && pointer->left->word_type == w_t_none)
		{
			pointer = pointer->left;
			while (pointer != NULL && pointer->op_type != o_t_none)
				pointer = pointer->right;
			if (pointer == NULL)
			{
				this->parsing_error = strdup("unexpected token |");
				return (FALSE);
			}
		}
	}
	if (pointer->need_a_file(pointer))
	{
		if (pointer->op_type == redirection || pointer->op_type == append
			|| pointer->op_type == heredoc)
		{
			if (pointer->output_file == NULL)
			{
				this->parsing_error = strdup("syntax error");
				return (FALSE);
			}
		}
		else
		{
			if (pointer->input_file == NULL)
			{
				this->parsing_error = strdup("syntax error");
				return (FALSE);
			}
		}
	}
	return (check_syntax(this, tmp->left) && check_syntax(this, tmp->right));
}
