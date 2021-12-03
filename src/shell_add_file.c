#include "minishell.h"

void	add_file(t_shell *this, t_node *head, t_token *token)
{
	if (this->parsing_error != NULL)
		return ;
	if (head->op_type == redirection)
	{
		head->output_file = token->to_file(token);
		if (head->output_file->open(head->output_file,
				O_WRONLY | O_CREAT | O_TRUNC, 0644))
			this->parsing_error = strdup(head->output_file->uri);
	}
	else if (head->op_type == append)
	{
		head->output_file = token->to_file(token);
		if (head->output_file->open(head->output_file,
				O_CREAT | O_APPEND | O_WRONLY, 0644))
			this->parsing_error = strdup(head->output_file->uri);
	}
	else if (head->op_type == input)
	{
		head->input_file = token->to_file(token);
		if (head->input_file->open(head->input_file, O_RDONLY, -1))
			this->parsing_error = strdup(head->input_file->uri);
	}
	else
	{
		head->eof = strdup(token->value);
		head->output_file = new_file("/tmp/B0N3_HEREDOC");
		if (head->output_file->open(head->output_file,
				O_RDONLY | O_CREAT, 0644))
			this->parsing_error = strdup(head->output_file->uri);
	}
	token->free(token);
}
