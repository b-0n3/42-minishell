//
// Created by Abdelouahad Ait hamd on 12/1/21.
//
#include <unistd.h>
#include "minishell.h"
#include <errno.h>

void exec_echo(t_shell *this, t_node *node)
{
    int in;
    int out;
    out = dup(STDOUT_FILENO);
    in  = dup(STDIN_FILENO);
    init_fds(node);
    cmd_echo(node);
    dup2(out ,STDOUT_FILENO);
    dup2(in , STDIN_FILENO);
    close(in);
    close(out);
    this->exit_code = 0;
}

void exec_cd(t_shell *this, t_node *node)
{
    int in;
    int out;
    out = dup(STDOUT_FILENO);
    in  = dup(STDIN_FILENO);
    init_fds(node);
    cmd_cd(this, node);
    dup2(out ,STDOUT_FILENO);
    dup2(in , STDIN_FILENO);
    close(in);
    close(out);
}

void exec_pwd(t_shell *this, t_node *node)
{
    int in;
    int out;
    out = dup(STDOUT_FILENO);
    in  = dup(STDIN_FILENO);
    init_fds(node);
    cmd_pwd(this);
    dup2(out ,STDOUT_FILENO);
    dup2(in , STDIN_FILENO);
    close(in);
    close(out);
    this->exit_code = 0;
}

void exec_export(t_shell *this, t_node *node)
{
    int in;
    int out;
    out = dup(STDOUT_FILENO);
    in  = dup(STDIN_FILENO);
    init_fds(node);
    cmd_export(this, node);
    dup2(out ,STDOUT_FILENO);
    dup2(in , STDIN_FILENO);
    close(in);
    close(out);
    this->exit_code = 0;
}

void exec_unset(t_shell *this, t_node *node)
{
    int in;
    int out;
    out = dup(STDOUT_FILENO);
    in  = dup(STDIN_FILENO);
    init_fds(node);
    cmd_unset(this, node);
    dup2(out ,STDOUT_FILENO);
    dup2(in , STDIN_FILENO);
    close(in);
    close(out);
    this->exit_code = 0;
}

void exec_env(t_shell *this, t_node *node)
{
    int in;
    int out;

    out = dup(STDOUT_FILENO);
    in = dup(STDIN_FILENO);
    init_fds(node);
    cmd_env(this);
    dup2(out, STDOUT_FILENO);
    dup2(in, STDIN_FILENO);
    close(in);
    close(out);
    this->exit_code = 0;
}

void exec_exit(t_shell *this, t_node *node)
{
    int in;
    int out;

    out = dup(STDOUT_FILENO);
    in = dup(STDIN_FILENO);
    init_fds(node);
    cmd_exit(this, node);
    dup2(out, STDOUT_FILENO);
    dup2(in, STDIN_FILENO);
    close(in);
    close(out);
}

t_string find_command_in_path(t_shell  *this,t_node *cmd)
{
    t_string path_variable;
    t_string path;
    t_array_iterator *iter;
    struct stat sb;

    if (this == NULL || cmd == NULL || cmd->value == NULL)
        return (NULL);
    if (strchr(cmd->value, '/') != NULL) {
        return strdup(cmd->value);
    }
            path_variable = (t_string)this->env.find_by_key(this->env, "PATH");
    if (path_variable == NULL)
        return (NULL);

    iter = split(path_variable, ':');
    while(iter->has_next(iter))
    {
        path = iter->next(iter);
        if (path != NULL)
        {
            path_variable = strdup(path);
            path_variable = ft_strjoin(path_variable, "/");
            path_variable = ft_strjoin(path_variable, cmd->value);
            if (stat(path_variable, &sb) == 0)
            {
                free(cmd->value);
                cmd->value = path_variable;
                iter->free(iter, &free);
                return cmd->value;
            }
            free(path_variable);
        }
    }
    iter->free(iter, &free);
    return NULL;
}

t_string *node_to_execv_args(t_node *node)
{
    t_string *ret;
    t_array_iterator  *iter;
    int i;

    if (node == NULL || node->word_type == w_t_none)
        return NULL;
    ret = malloc(sizeof(char *) * (node->args.index + 2));
    i = 0;
    iter = node->args.iterator(&node->args);
    ret[i] = strdup(node->value);
    i++;
    while (iter->has_next(iter))
    {
        ret[i] = strdup(((t_token *)iter->next(iter))->value);
        i++;
    }
    ret[i] = NULL;
    return (ret);
}

void     exec_other(t_shell *this, t_node  *node)
{
    t_string *cmd = node_to_execv_args(node);

     init_fds(node);
     find_command_in_path(this, node);

    if (execve(node->value, cmd , this->env_to_arr(this)) == -1) {
        if (errno != 13)
        {
            printf("minishell: %s command not found\n", node->value);
            exit(127);
        }
        perror("minishell");
        exit(126);
    }
}







