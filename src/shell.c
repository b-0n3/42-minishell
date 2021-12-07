

#include "minishell.h"
extern int g_mood;


t_bool has_next_token(t_shell *this)
{
	return this->cursor <this->command_len;
}

char *join_command(t_string origin, t_string next)
{
	size_t len;
	t_string result;

	if (next == NULL || origin == NULL)
		return NULL;
	len = strlen(origin) + strlen(next);
	result = malloc(len + 3);
	if (result == NULL)
		return  NULL;
	ft_strcpy(result, origin);
	ft_strcpy(result+ ft_strlen(origin), "\n");
	ft_strcpy(result+ ft_strlen(result), next);

	return result;
}

t_bool shell_quot_unclosed(t_shell *this)
{
	return (this->quot || this->dqout);
}

t_token *pre_get_next_token(t_shell *this)
{
	t_token *token;

	token = this->get_next_token(this);
	return token;
}

void *str_to_token(void *str)
{
    return new_token(strdup((t_string )str), word);
}

t_node *token_to_node(t_token *this)
{
	t_node  *node;
    t_array_iterator  *iter;

	node = new_node();
	if (node == NULL)
		return (NULL);
	node->value = strdup(this->value);
	if (this->type == word) {
        node->word_type = command;
        if (this->start_with != '\"' && this->start_with != '\'') {
            iter = split(this->value, ' ');
            if (iter->list->index > 1) {
                free(node->value);
                node->value = strdup((char *)iter->next(iter));
                while(iter->has_next(iter))
                  node->args.push(&node->args, iter->do_on_next(iter, &str_to_token), sizeof(t_token));
            }
            iter->free(iter, &free);
        }
    }
    else
		node->op_type = find_operation_type(this->value);
	return node;
}

t_node  *handle_operator(t_shell  *this, t_token  *token, t_node *head)
{
	t_node  *operator;
	if (token == NULL)
		return head;
	operator = token->to_node(token);

	if (operator->op_type == pipeline)
	{
		if (head != NULL)
		{
			operator->left = head;
            head->isleft = TRUE;
			head->parent = operator;
			head = operator;
		} else
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
                operator->parent = head;
                operator->isleft = FALSE;

			}
            else
            {
                operator->isleft = FALSE;
                head->right =operator;
                head->right->parent = head;
                operator->parent = head;
            }
		}
		else
		{
			operator->left = head;
            head->isleft= TRUE;
			head->parent = operator;
			head = operator;
		}
	}else
		head = operator;
    token->free(token);
	return head;
}




t_node *handle_word(t_shell *this, t_token *token ,t_node *head)
{
	t_node *com;

    token->expand(token,this->exit_code, this->env);
	if (head != NULL && head->right != NULL && head->right->need_a_file(head->right))
	{
        add_file(this,head->right, token);
	}
	else if (head != NULL && head->need_a_file(head))
		add_file(this,head, token);
	else if (head == NULL || head->op_type == pipeline  )
	{
		com = token->to_node(token);
		token->free(token);
		if (head != NULL)
		{
			if (head->left == NULL) {
                head->left = com;
                com->isleft = TRUE;
            }
              else {
                head->right = com;
                com->isleft = FALSE;
            }
                com->parent = head;
		}else
			head = com;
		while (1)
		{
			token = pre_get_next_token(this);
            if (token != NULL)
               token->expand(token,this->exit_code, this->env);
			if (token == NULL || token->type != word)
				return handle_operator(this, token ,head);
			com->args.push(&com->args, token , sizeof(t_token));
		}
	}
	return head;
}


t_bool shell_parse(t_shell *this) {
	t_token *token;
	t_node *head;

	head = NULL;
	while (this->has_next_token(this))
	{
		token = pre_get_next_token(this);
        if (token == NULL)
            break;
		if (token->type == word)
			head = handle_word(this, token, head);
		else
			head = handle_operator(this , token, head);
	}
    this->head = head;
    if (this->unclosed(this))
    {
        this->parsing_error = strdup("syntax error");
        return (FALSE);
    }
	return  check_syntax(this, head); 
}

void shell_loop(t_shell *this)
{
	t_string line;

	line = readline("IM->B0N3$>");
	while (line != NULL) {
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
