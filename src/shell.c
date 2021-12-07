/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 03:53:23 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/07 03:54:56 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_mood;

char	*join_command(t_string origin, t_string next)
{
	size_t		len;
	t_string	result;

	if (next == NULL || origin == NULL)
		return (NULL);
	len = strlen(origin) + strlen(next);
	result = malloc(len + 3);
	if (result == NULL)
		return (NULL);
	ft_strcpy(result, origin);
	ft_strcpy(result + ft_strlen(origin), "\n");
	ft_strcpy(result + ft_strlen(result), next);
	return (result);
}

t_bool	shell_quot_unclosed(t_shell *this)
{
	return (this->quot || this->dqout);
}

t_bool	shell_parse(t_shell *this)
{
	t_token		*token;
	t_node		*head;

	head = NULL;
	while (this->has_next_token(this))
	{
		token = this->get_next_token(this);
		if (token == NULL)
			break ;
		if (token->type == word)
			head = handle_word(this, token, head);
		else
			head = handle_operator(this, token, &head);
	}
	this->head = head;
	if (this->unclosed(this))
	{
		this->parsing_error = strdup("syntax error");
		return (FALSE);
	}
	return (check_syntax(this, head));
}

void	shell_loop(t_shell *this)
{
	t_string	line;

	line = readline("IM->B0N3$>");
	while (line != NULL)
	{
		this->init(this, line);
		if (this->parse(this))
		{
			this->execute(this);
		}
		else
			perror(this->parsing_error);
		add_history(line);
		free(this->commmand);
		line = readline("IM->B0N3$>");
	}
	this->free(this);
}
