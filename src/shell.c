//
// Created by Abdelouahad Ait hamd on 11/8/21.
//
#include "minishell.h"
t_string split_env(t_string env)
{
	int i;
	i = 0;
	while (env[i] != '\0' && env[i] != '=')
		i++;
	return strndup(env, i);
}

t_key_map *env_to_key_map(t_string str)
{
	t_key_map *map;

	map = malloc(sizeof(t_key_map));
	if (map == NULL)
		return (NULL);
	map->key = split_env(str);
	map->value = strdup( str + strlen(map->key) + 1);
	return map;
}

void push_env(t_array_list *list,t_string  *env)
{
    list->push(list, env_to_key_map("vv=ls -al") , sizeof (t_key_map *));
	while (*env != NULL)
	{
		list->push(list, env_to_key_map(*env), sizeof(t_key_map *));
		env++;
	}
}

void create_shell(t_shell *this, t_string *env)
{
	this->parse = &shell_parse;
	new_array_list(&this->env, 10, sizeof(char *));
	push_env(&this->env, env);
	this->fresh = TRUE;
	this->init = &init_shell;
	this->get_next_token = &shell_get_next_token;
	this->has_next_token = &has_next_token;
	this->unclosed = &shell_quot_unclosed;
	this->loop = &shell_loop;
	this->free = &shell_free;
}

void init_shell(t_shell *this ,t_string line)
{
	if (this != NULL) {
		if (!this->fresh)
		{
			//this->tokens->free(this->tokens, &free);
			//  free(this->tokens);
			//  this->nodes->free(this->nodes, &free);
			// free(this->nodes);
			//    free(this->commmand);
		}
		this->tokens = malloc(sizeof(t_array_list));
		new_array_list(this->tokens, 10, sizeof (t_token));
		this->nodes = malloc(sizeof(t_array_list));
		new_array_list(this->nodes, 10, sizeof (t_node));
		this->cursor = 0;
		this->l_cursor = 0;
		this->fresh = 0;
		this->commmand = line;
		this->command_len = strlen(this->commmand);
		//        if (!this->unclosed(this)) {
		this->dqout = FALSE;
		this->quot = FALSE;
		//        }
	}

}
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
	strcpy(result, origin);
	strcpy(result+ strlen(origin), "\n");
	strcpy(result+ strlen(result), next);

	return result;
}

t_bool shell_quot_unclosed(t_shell *this)
{
	return (this->quot || this->dqout);
}

void print_token(void *item)
{
	t_token *m;

	if (item != NULL)
	{
		m = (t_token*) item;
		printf("\t{\n\t\"type\":\"%s\",\n \t\"value\":\"%s\"\n}\n", m->type == op? "OP": "WORD", m->value);
	}
}
t_token *pre_get_next_token(t_shell *this)
{
	t_token *token = NULL;
	char *next;

	if (this->unclosed(this)) {

		next = join_command(this->commmand, readline(">"));
		if (next == NULL)
			return NULL;
		this->init(this, next);
		return  pre_get_next_token(this);
	}
	token = this->get_next_token(this);
	return token;
}

void split_it(t_array_list *this, t_string cmd, char ch)
{
    int dq;
    int q;
    int cursor;
    int l_cursor;

    cursor = 0;
    l_cursor = 0;
    q = 0;
    dq = 0;
    while(cmd[cursor] != '\0')
    {
        if (cmd[cursor] == '\"' && !q)
           dq = !dq;
        if (cmd[cursor] == '\'' && !dq)
            q = !q;
        if (!q && !dq && cmd[cursor] == ch)
        {
            this->push(this, strndup(cmd + l_cursor, cursor - l_cursor ), sizeof(char *));
            while (cmd[cursor] == ch)
                cursor++;
            l_cursor = cursor;
        }
        cursor++;
    }
    if (l_cursor < cursor)
        this->push(this, strndup(cmd + l_cursor, cursor - l_cursor ), sizeof(char *));
}

t_array_iterator *split(t_string cmd, char ch)
{
    t_array_list  *list;

    if (cmd == NULL)
        return (NULL);
    list = malloc(sizeof(t_array_list));
    new_array_list(list, 2, sizeof(char *));
    if (list == NULL)
      return (NULL);
    split_it(list, cmd , ch);
    return list->iterator(list);
}

void *str_to_token(void *str)
{
    return new_token((t_string )str, word);
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
                node->value = (char *) iter->next(iter);
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

void add_file(t_node *head, t_token *token)
{
	if (head->op_type == redirection || head->op_type == append)
		head->output_file = token->to_file(token);
	else if (head->op_type == input)
		head->input_file = token->to_file(token);
	else
	{
		head->eof = strdup(token->value);
		head->output_file = new_file("/tmp/B0N3_HEREDOC");
	}
	token->free(token);
}
// cat hell | wc -l >> f1 | grep hell.js > f1
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
				head->right->parent = operator;
				head->right = operator;
			}
		}
		else
		{
			operator->left = head;
			head->parent = operator;
			head = operator;
		}
	}else
		head = operator;

	return head;
}

t_bool is_env_variables(t_string str)
{
	t_bool  dq;
	t_bool  q;
	int i;

	dq = FALSE;
	q = FALSE;
	i = 0;
	while (str[i]!= '\0')
	{
		if (!dq && str[i] == '\'')
			q = !q;
		if (!q && str[i] == '\"')
			dq = !dq;
		if(!q && str[i] == '$')
			return TRUE;
		i++;
	}
	return FALSE;
}


t_node *handle_word(t_shell *this, t_token *token ,t_node *head)
{
	t_node *com;

    token->expand(token, this->env);
	if (head != NULL && head->right != NULL && head->right->need_a_file(head->right))
	{
		add_file(head->right, token);
	}
	else if (head != NULL && head->need_a_file(head))
		add_file(head, token);
	else if (head == NULL || head->op_type == pipeline  )
	{
		com = token->to_node(token);
		token->free(token);
		if (head != NULL)
		{
			if (head->left == NULL)
				head->left = com;
			else
				head->right = com;
			com->parent = head;
		}else
			head = com;
		while (1)
		{
			token = pre_get_next_token(this);
            if (token != NULL)
               token->expand(token, this->env);
			if (token == NULL || token->type != word)
				return handle_operator(this, token ,head);
			com->args.push(&com->args, token , sizeof(t_token));
		}
	}
	return head;
}

//t_bool check_pipeline_sides(t_shell *this, t_node *pipe) {
//    t_bool ret;
//
//    ret = !(pipe->right == NULL || pipe->left == NULL);
//    if (!ret  && pipe->right)
//
//    return (ret);
//}


t_bool check_syntax(t_shell  *this, t_node *pointer)
{
    if (pointer == NULL)
        return TRUE;
    if(pointer->op_type == pipeline)
    {
        if (pointer->left == NULL || pointer->right == NULL)
        {
            this->parsing_error = strdup("unexpected token |");
            return (FALSE);
        }
        if (pointer->right != NULL && pointer->right->word_type == w_t_none)
        {
            pointer = pointer->right;
            while(pointer != NULL && pointer->op_type != o_t_none)
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
            while(pointer != NULL && pointer->op_type != o_t_none)
                pointer = pointer->right;
            if (pointer == NULL)
            {
                this->parsing_error = strdup("unexpected token |");
                return (FALSE);
            }
        }

    }
	return check_syntax(this, pointer->left) && check_syntax(this, pointer->right);
}

t_bool shell_parse(t_shell *this) {
	t_token *token;
	t_node *head;

	head = NULL;
	while (this->has_next_token(this))
	{
		token = pre_get_next_token(this);
		if (token == NULL && this->has_next_token(this))
		{
			node_free(head);
			head = NULL;
			continue;
		}
		if (token->type == word)
			head = handle_word(this, token, head);
		else
			head = handle_operator(this , token, head);
	}
	this->head = head;
	return  check_syntax(this, head);
}

void print_node(t_node *node)
{
	if (node == NULL)
		return;
	printf("{\n\t\"value\": \"%s\",\n",node->value);
	printf("\n\t\"type\": \"%d\",\n",node->op_type);
	printf("\n\t\"value\": \"%d\",\n",node->word_type);
	printf("\n\t\"type\": \"%d\",\n",node->command_type);
	printf("\n\t\"args\": \n");
	node->args.foreach(&node->args, &print_token);
	printf("}");
	print_node(node->left);
	print_node(node->right);

}

void shell_loop(t_shell *this)
{
	t_string line;

	line = readline("IM->B0N3$>");
	while (line != NULL && strcmp(line,  "exit") != 0) {
		this->init(this, line);
		if (this->parse(this))
		{

		}
		else
			printf(this->parsing_error);
		// execute command
		line = readline("IM->B0N3$>");
	}
}

void shell_free(t_shell *this)
{

}

