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
    this->execute = &shell_execute;
	this->loop = &shell_loop;
	this->free = &shell_free;
}

void init_shell(t_shell *this ,t_string line)
{
	if (this != NULL) {
		if (!this->fresh)
		{
            if (this->parsing_error != NULL)
                free(this->parsing_error);
		}
//		this->tokens = malloc(sizeof(t_array_list));
//		new_array_list(this->tokens, 10, sizeof (t_token));
//		this->nodes = malloc(sizeof(t_array_list));
//		new_array_list(this->nodes, 10, sizeof (t_node));

        this->exit_code = -1;
		this->cursor = 0;
		this->l_cursor = 0;
		this->fresh = 0;
		this->commmand = line;
        this->parsing_error = NULL;
		this->command_len = strlen(this->commmand);
		//        if (!this->unclosed(this)) {
		this->dqout = FALSE;
		this->quot = FALSE;
        init_exec_builtins(this);
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

t_bool file_open(t_file *this, int mode, int perms)
{
    if (perms > 0)
        this->fd = open(this->uri ,mode, perms);
    else
        this->fd = open(this->uri ,mode);
    this->exception =  this->fd < 0;
    return this->exception;
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
                node->value = (char *)iter->next(iter);
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

void add_file(t_shell  *this, t_node *head, t_token *token)
{
    if (this->parsing_error != NULL)
            return ;
	if (head->op_type == redirection) {
        head->output_file = token->to_file(token);
        if (head->output_file->open(head->output_file,O_WRONLY| O_CREAT | O_TRUNC,0644))
            this->parsing_error = strdup(head->output_file->uri);
    }
    else if (head->op_type == append)
    {
        head->output_file = token->to_file(token);
         if (head->output_file->open(head->output_file, O_CREAT | O_APPEND | O_WRONLY, 0644))
            this->parsing_error = strdup(head->output_file->uri);
    }
    else if (head->op_type == input) {
        head->input_file = token->to_file(token);
        if (head->input_file->open(head->input_file, O_RDONLY, -1))
            this->parsing_error = strdup(head->input_file->uri);
    }
    else
	{
		head->eof = strdup(token->value);
		head->output_file = new_file("/tmp/B0N3_HEREDOC");
        if (head->output_file->open(head->output_file,O_RDONLY | O_CREAT, 0644))
            this->parsing_error = strdup(head->output_file->uri);
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
                operator->isleft = FALSE;

			}
            else
            {
                operator->isleft = FALSE;
                head->right =operator;
                head->right->parent = head;
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

	return head;
}




t_node *handle_word(t_shell *this, t_token *token ,t_node *head)
{
	t_node *com;

    token->expand(token, this->env);
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
               token->expand(token, this->env);
			if (token == NULL || token->type != word)
				return handle_operator(this, token ,head);
			com->args.push(&com->args, token , sizeof(t_token));
		}
	}
	return head;
}

//t_bool check_pipeline_sides(t_shell *this, t_node *_pipe) {
//    t_bool ret;
//
//    ret = !(_pipe->right == NULL || _pipe->left == NULL);
//    if (!ret  && _pipe->right)
//
//    return (ret);
//}


t_bool check_syntax(t_shell  *this, t_node *pointer)
{
    t_node  *tmp;

    tmp = pointer;
    if (pointer == NULL )
         return TRUE;
    if (this != NULL && this->parsing_error != NULL)
        return FALSE;
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
    if (pointer->need_a_file(pointer))
    {
        if (pointer->op_type == redirection || pointer->op_type == append || pointer->op_type == heredoc) {
            if (pointer->output_file == NULL) {
                this->parsing_error = strdup("syntax error");
                return (FALSE);
            }
        }
        else
            if (pointer->input_file == NULL)
            {

                this->parsing_error = strdup("syntax error");
                return (FALSE);
            }
    }
	return check_syntax(this, tmp->left) && check_syntax(this, tmp->right);
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
	return  check_syntax(this, head) ;//&& check_file_syntax(this->head);
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
exec_v *find_function(t_shell  *this , t_string value)
{
    exec_v *func = (exec_v *)this->exec_pool.find_by_key(this->exec_pool, value);
    if(func ==  NULL)
        func = &exec_other;
    return func;
}

t_bool  luanch(t_shell *this, t_node *head)
{
    if (head == NULL)
        return TRUE;
    if (head->op_type == pipeline) {
        pipe(head->p);
    }
    else if (head->word_type == command)
    {
        head->pid = fork();
        if (head->pid == 0)
        {
            find_function(this, head->value)(this, head);
            exit(1);
        }else {
            this->last_one = head->pid;
            printf("this is pid {%d} for command %s", this->last_one, head->value);
            if (head->parent != NULL) {
                if (head->parent->op_type == pipeline) {
                    if (!head->isleft) {
                        close(head->parent->p[1]);
                        close(head->parent->p[0]);
                    }
                }
            }
        }

    }
    return luanch(this, head->left) && luanch(this, head->right);
}

t_bool close_fds(t_shell *this, t_node *head)
{
    if (head == NULL)
        return TRUE;
    if (head->op_type == pipeline)
    {
        close(head->p[0]);
        close(head->p[1]);
    }
    return close_fds(this, head->left) && close_fds(this, head->right);
}
t_bool  wait_for_all(t_shell *this, t_node *head)
{
    int exi;

    if (head == NULL)
        return TRUE;

    if (head->word_type == command && this->last_one != head->pid) {
        waitpid(head->pid, &exi, 0);
    }
    return wait_for_all(this,head->left) && wait_for_all(this, head->right);
}


void shell_execute(t_shell *this){
    luanch(this,this->head);
    wait_for_all(this, this->head);
    waitpid(this->last_one, &this->exit_code, 0);

   close_fds(this, this->head);
}

void shell_loop(t_shell *this)
{
	t_string line;

	line = readline("IM->B0N3$>");
	while (line != NULL && strcmp(line,  "exit") != 0) {
		this->init(this, line);
		if (this->parse(this))
		{
            this->execute(this);
		}
		else
			perror(this->parsing_error);
		// execute command
		line = readline("IM->B0N3$>");
	}
    // @Todo: call exit
}

void shell_free(t_shell *this)
{

}

