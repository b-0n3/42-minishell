/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_handle_word.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 06:46:39 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 06:58:34 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_mood;

t_node	*handle_word(t_shell *this, t_token *token, t_node *head)
{
	t_node	*com;

	token->expand(token, this->exit_code, this->env);
	if (head != NULL && head->right != NULL
		&& head->right->need_a_file(head->right))
		add_file(this, head->right, token);
	else if (head != NULL && head->need_a_file(head))
		add_file(this, head, token);
	else if (head == NULL || head->op_type == pipeline)
	{
		com = token->to_node(token);
		token->free(token);
		if (head != NULL)
		{
			if (head->left == NULL)
			{
				head->left = com;
				com->isleft = TRUE;
			}
			else
			{
				head->right = com;
				com->isleft = FALSE;
			}
				com->parent = head;
		}
		else
			head = com;
		while (1)
		{
			token = pre_get_next_token(this);
			if (token != NULL)
				token->expand(token, this->exit_code, this->env);
			if (token == NULL || token->type != word)
				return (handle_operator(this, token, head));
			com->args.push(&com->args, token, sizeof(t_token));
		}
	}
	return (head);
}

//t_bool check_pipeline_sides(t_shell *this, t_node *_pipe) {
//	t_bool ret;
//
//	ret = !(_pipe->right == NULL || _pipe->left == NULL);
//	if (!ret  && _pipe->right)
//
//	return (ret);
//}

t_bool	check_syntax(t_shell *this, t_node *pointer)
{
	t_node	*tmp;

	tmp = pointer;
	if (pointer == NULL )
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
			while (pointer != NULL && pointer->op_type != o_t_none
				&& !pointer->need_a_file(pointer))
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
			while (pointer != NULL && pointer->op_type != o_t_none
				&& !pointer->need_a_file(pointer))
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
