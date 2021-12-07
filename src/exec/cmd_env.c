#include "minishell.h"

extern int mood;

void print_env(void *item)
{
    t_key_map *map;
    map = (t_key_map *) item;

    if (map->value != NULL)
    {
        write(STDOUT_FILENO, map->key,strlen(map->key));
        write(STDOUT_FILENO, "=", 1);
        write(STDOUT_FILENO, map->value, strlen((char *) map->value));
        write(STDOUT_FILENO,"\n", 1);
    }
}

void	cmd_env(t_shell *this)
{
	this->env.foreach(&this->env, &print_env);

    if(mood == 1)
        exit(0);
}
t_bool env_cond(void *item, void *item2)
{
    t_string key;
    t_key_map *map;

    map = (t_key_map *) item;
    key = (t_string) item2;
    if (map == NULL)
        return FALSE;
    if (map->key == NULL ||  key == NULL)
        return FALSE;
    return (!strcmp((char *)map->key, key));
}

void unset_token(t_shell *this, t_token *token)
{
    t_key_map  *m;
    size_t i;

    i = 0;
    if (token->value != NULL)
    {
        while (i < this->env.index)
        {
            m = this->env.get(&this->env, i);
            if (m != NULL)
            {
                if (env_cond(m, token->value))
                {
                    this->env.arr[i] = NULL;
                }
            }
            i++;
        }
    }
}


void	cmd_unset(t_shell *this,t_node *head) {
    t_array_iterator *iterator;
    t_token  *token;

    iterator = head->args.iterator(&head->args);
    while(iterator->has_next(iterator))
    {
        token = (t_token *) iterator->next(iterator);
        unset_token(this, token);
    }

    iterator->free(iterator, NULL);
}
void print_export(void *item)
{
    t_key_map *map;
    map = (t_key_map *) item;
    if (map == NULL)
        return;

    write(STDOUT_FILENO,"declare -x ", 11);
    write(STDOUT_FILENO, map->key,strlen(map->key));
    if (map->value != NULL)
    {
        write(STDOUT_FILENO, "=\"", 2);
        write(STDOUT_FILENO, map->value, strlen((char *) map->value));
        write(STDOUT_FILENO, "\"", 1);
    }
    write(STDOUT_FILENO,"\n", 1);

}

t_bool  check_ident(t_key_map *map)
{
    int i;

    i = 0;
    if (map == NULL || map->key == NULL)
        return FALSE;
    while(map->key[i] == ' ')
        i++;

    return (isdigit(map->key[i]) || (map->key[i] != '_' && !isalpha(map->key[i])));

}
t_bool  end_with_(t_string str, char c)
{
    int i;
    i = 0;
    while (str[i] != '\0')
        i++;
    return str[i] = c;
}
void	cmd_export(t_shell *this, t_node *node)
{
    t_key_map  *map;
    t_string    old_val;

    old_val = NULL;
    if (node->args.index == 0)
        this->env.foreach(&this->env, &print_export);
    else
    {
        map = env_to_key_map(((t_token*)node->args.get(&node->args,0))->value);
        if (check_ident(map))
        {
            this->exit_code = 1;
            printf("minishell: export: not a valid identifier\n");
            my_free(map->key);
            my_free(map);
            my_free(map->value);
            if (mood == 1)
                exit(1);
            return;
        }
        this->env.replace_by_key(&this->env, map->key, map->value, &free);
        free(map->key);
        free(map);
    }
    if (mood == 1)
        exit(0);
    this->exit_code = 0;
}
