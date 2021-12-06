//
// Created by Abdelouahad Ait hamd on 12/2/21.
//
#include "minishell.h"

void init_exec_builtins(t_shell *this)
{
    if (this != NULL)
    {
        new_array_list(&this->exec_pool, 10 , sizeof(t_key_map));
        this->exec_pool.is_map = TRUE;
        this->exec_pool.push(&this->exec_pool, new_func_map("echo", &exec_echo), sizeof (t_key_map));
        this->exec_pool.push(&this->exec_pool, new_func_map("cd", &exec_cd), sizeof (t_key_map));
        this->exec_pool.push(&this->exec_pool, new_func_map("pwd", &exec_pwd), sizeof (t_key_map));
        this->exec_pool.push(&this->exec_pool, new_func_map("export", &exec_export), sizeof (t_key_map));
        this->exec_pool.push(&this->exec_pool, new_func_map("unset", &exec_unset), sizeof (t_key_map));
        this->exec_pool.push(&this->exec_pool, new_func_map("env", &exec_env), sizeof (t_key_map));
        this->exec_pool.push(&this->exec_pool, new_func_map("exit", &exec_exit), sizeof (t_key_map));

   }

}

void init_fds(t_node *node)
{
    t_node *tmp;
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
                    tmp = tmp->parent;
                }
                if (tmp->parent != NULL && tmp->parent->op_type == pipeline)
                {
                    close(tmp->parent->p[1]);
                    dup2(tmp->parent->p[0], STDIN_FILENO);
                    close(tmp->parent->p[0]);
                }
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
}

t_key_map  *new_func_map(t_string key, exec_v *func){
    t_key_map  *this;

    this = malloc(sizeof(t_func_map));
    if (this == NULL)
            return NULL;
    this->key = strdup(key);
    this->value = func;
    return (this);
}