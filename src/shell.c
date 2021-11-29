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

/**
 *   @todo : write the fucking code
 */



t_node *token_to_node(t_token *this)
{
    t_node  *node;

    node = new_node();
    if (node == NULL)
            return (NULL);
    node->value = strdup(this->value);
    if (this->type == word)
        node->word_type = command;
    else
        node->op_type = find_operation_type(this->value);
    return node;
}

t_node  *handle_operator(t_shell  *this, t_token  *token, t_node *head)
{
    t_node  *operator;
    if (token == NULL)
        return head;
    operator = new_node();
    operator->op_type = find_operation_type(token->value);
    operator->value = strdup(token->value);
    if(head != NULL)
    {
        if (operator->op_type == pipeline && head->op_type != pipeline)
        {
            head->parent = operator;
            operator->left = head;
            head = operator;
        }
        else if (operator->op_type == pipeline)
        {
            head->parent = operator;
           operator->left = head;
        }
        else
        {
            operator->parent = head;
            operator->left = head->right;
            if (operator->left != NULL)
                operator->left->parent = operator;
            head->right = operator;
        }
    }
    else
        head = operator;
    return head;
}

t_node *handle_word(t_shell *this, t_token *token ,t_node *head)
{
    t_node *com;

    if (head == NULL || (head->op_type == pipeline  && head->right != NULL))
    {
        com = new_node();
        com->word_type = command;
        com->value = strdup(token->value);
        if (head != NULL) {
            if(head->left != NULL)
                 head->left = com;
            else
                head->right = com;
            com->parent = head;
        }
        else
            head = com;
        token->free(token);
        while(1)
        {
            token = pre_get_next_token(this);
            if (token == NULL || token->type != word)
                return handle_operator(this, token, head);
            com->args.push(&com->args, token , sizeof(t_token));
        }
    }
    else if (head->need_a_file(head) || (head->right != NULL  && head->right->need_a_file(head->right)))
    {
        com = head->right != NULL ? head->right : head;
        if (com->op_type == redirection || com->op_type == append)
            com->output_file = token->to_file(token);
        else if (com->op_type == heredoc)
            com->output_file = new_file("/tmp/b0n3_heredoc");
        else
            com->input_file = token->to_file(token);
    }
    return head;
}


void shell_parse(t_shell *this) {
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
    print_node(head);
}

//    while (this->has_next_token(this)) {
//        token = this->get_next_token(this);
//        if (token == NULL && this->unclosed(this)) {
//
//            next = join_command(this->commmand, readline(">"));
//            if (next == NULL)
//                break;
//            this->init(this, next);
//            continue;
//        } else if (token == NULL)
//            break;
//        //
////        this->tokens->push(this->tokens, token, sizeof(t_token));
//    }
//   // this->tokens->foreach(this->tokens, &print_token);
//    iterator = this->tokens->iterator(this->tokens);
//    t_node  *pre;
//    t_node  *tmp;
//    if (iterator != NULL)
//    {
//        pre = NULL;
//        while (iterator->has_next(iterator))
//        {
//           tmp =  iterator->do_on_next_p_(iterator, (void *(*)(void *, void *, void *)) &token_to_node, pre);
//           if (tmp != NULL) {
//               pre = tmp;
//           }
//           }
//
//        while (pre->parent != NULL)
//                pre = pre->parent;
//
//
//        print_node(pre);
  //  }

//}

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

    line = readline("$>");
    while (line != NULL && strcmp(line,  "exit") != 0)
    {
        this->init(this, line);
        this->parse(this);
        // execute command
        line = readline("$>");
    }
}

void shell_free(t_shell *this)
{

}

