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
    this->loop = &shell_loop;
    this->free = &shell_free;
}

void init_shell(t_shell *this ,t_string line)
{
    if (this != NULL) {
        if (!this->fresh)
        {
            // todo: free old values; and set fresh to @FALSE
        }
        this->tokens = malloc(sizeof(t_array_list));
        new_array_list(this->tokens, 10, sizeof (t_token));
        this->nodes = malloc(sizeof(t_array_list));
        new_array_list(this->nodes, 10, sizeof (t_node));
        this->cursor = 0;
        this->l_cursor = 0;
        this->commmand = line;
        this->dqout = FALSE;
        this->quot = FALSE;
    }

}

void shell_parse( t_shell *this)
{
}


void shell_loop(t_shell *this)
{
    t_string line;

    line = readline("IM->B0N3$>");
    while (line != NULL && !strcmp(line,"exit"))
    {
        this->init(this,line);
        this->parse(this);
        // execute command
        line = readline(" $>");
    }
}

void shell_free(t_shell *this)
{

}

