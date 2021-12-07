/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 03:27:34 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/07 03:30:54 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	check_file(t_shell *this, t_node *pointer)
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
	return (TRUE);
}

t_bool	check_pipeline(t_shell *this, t_node *pointer)
{
	if (pointer->right != NULL && pointer->right->word_type == w_t_none)
	{
		pointer = pointer->right;
		while (pointer != NULL && pointer->op_type != o_t_none
			&& !pointer->need_a_file(pointer))
			pointer = pointer->left;
		if (pointer == NULL)
		{
			this->parsing_error = ft_strdup("unexpected token |");
			return (FALSE);
		}
	}
	if (pointer->left != NULL && pointer->left->word_type == w_t_none)
	{
		pointer = pointer->left;
		while (pointer != NULL && pointer->op_type != o_t_none
			&& !pointer->need_a_file(pointer))
			pointer = pointer->right;
		if (pointer == NULL)
		{
				this->parsing_error = ft_strdup("unexpected token |");
			return (FALSE);
		}
	}
	return (TRUE);
}

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
		if (!check_pipeline(this, pointer))
			return (FALSE);
	}
	if (pointer->need_a_file(pointer))
		if (!check_file(this, pointer))
			return (FALSE);
	return (check_syntax(this, tmp->left) && check_syntax(this, tmp->right));
}
