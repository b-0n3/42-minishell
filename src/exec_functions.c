//
// Created by Abdelouahad Ait hamd on 12/1/21.
//
#include <unistd.h>
#include "minishell.h"
#include <errno.h>

void exec_echo(t_shell *this, t_node *node)
{
    printf("echo command \n");
}

void exec_cd(t_shell *this, t_node *node)
{
    cmd_cd(this, node);
}

void exec_pwd(t_shell *this, t_node *node)
{
    cmd_pwd(this);
    node = node;
}

void exec_export(t_shell *this, t_node *node)
{
    printf("export command \n");
}

void exec_unset(t_shell *this, t_node *node)
{
    printf("unset command \n");
}

void exec_env(t_shell *this, t_node *node)
{
    printf("env command \n");
}

void exec_exit(t_shell *this, t_node *node)
{
    printf("exit command \n");
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

extern char **environ;

void     exec_other(t_shell *this, t_node  *node)
{
    t_string *cmd = node_to_execv_args(node);
    t_node  *tmp;
    if (node->parent != NULL) {
        if (node->isleft)
        {
            if (node->parent->op_type == pipeline) {
                close(node->parent->p[0]);
                dup2(node->parent->p[1], STDOUT_FILENO);
                close(node->parent->p[1]);
            }
            else if (node->parent->op_type == input)
            {
                dup2(node->parent->input_file->fd, STDIN_FILENO);
                close(node->parent->input_file->fd);
            }
            else if (node->parent->op_type == heredoc)
            {
                dup2(node->parent->output_file->fd, STDIN_FILENO);
                close(node->parent->output_file->fd);
                if (node->parent->parent !=NULL)
                {
                    if (node->parent->parent->op_type == pipeline)
                    {
                        close(node->parent->parent->p[0]);
                        dup2(node->parent->parent->p[1], STDOUT_FILENO);
                        close(node->parent->parent->p[1]);
                    }
                    else if (node->parent->parent->op_type == redirection || node->parent->parent->op_type == append)
                    {
                        tmp = node->parent->parent;
                        while (tmp->parent != NULL && tmp->need_a_file(tmp))
                            tmp = tmp->parent;
                        dup2(tmp->output_file->fd, STDOUT_FILENO);
                        close(tmp->output_file->fd);
                    }

                }
            }
           else if (node->parent->op_type == redirection || node->parent->op_type == append) {
                tmp = node->parent;

                while (tmp->parent != NULL && tmp->parent->need_a_file(tmp->parent)) {
                    fprintf(stderr, "loop %s \n", tmp->output_file->uri);
                    tmp = tmp->parent;
                }
                    if (tmp->parent != NULL && tmp->parent->op_type == pipeline)
                {
                    fprintf(stderr, "sadfsadf %s \n", tmp->parent->value);
                    close(tmp->parent->p[1]);
                    dup2(tmp->parent->p[0], STDIN_FILENO);
                    close(tmp->parent->p[0]);
                }
                fprintf(stderr, "losadfop %s \n", tmp->output_file->uri);
                dup2(tmp->output_file->fd, STDOUT_FILENO);
              //  close(tmp->output_file->fd);
            }

        }
        else
        {
            close(node->parent->p[1]);
            dup2(node->parent->p[0], STDIN_FILENO);
            close(node->parent->p[0]);
            if (node->parent->parent != NULL)
            {
                close(node->parent->parent->p[0]);
                dup2(node->parent->parent->p[1], STDOUT_FILENO);
                close(node->parent->parent->p[1]);
            }

        }
    }
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







