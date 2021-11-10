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
            this->tokens->free(this->tokens, &free);
            free(this->tokens);
            this->nodes->free(this->nodes, &free);
            free(this->nodes);
            free(this->commmand);
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
        printf("data: {\n\"type\":\"%s\",\n \"value\":\"%s\"\n}\n", m->type == op? "OP": "WORD", m->value);
    }
}
void shell_parse( t_shell *this)
{
    t_token *token;
    t_string next;
    while (this->has_next_token(this))
    {
        token = this->get_next_token(this);
        if (token == NULL && this->unclosed(this))
        {

            next = join_command(this->commmand, readline(">"));
            if (next == NULL)
                break;
            this->init(this, next);
            continue;
        }
        else if (token == NULL)
            break;
        this->tokens->push(this->tokens, token, sizeof(t_token));
    }
    this->tokens->foreach(this->tokens, &print_token );

}


void shell_loop(t_shell *this)
{
    t_string line;

    line = readline("IM->B0N3$>");
    while (line != NULL && strcmp(line,  "exit") != 0)
    {
        this->init(this, line);
        this->parse(this);
        // execute command
        line = readline("IM->B0N3$>");
    }
}

void shell_free(t_shell *this)
{

}

