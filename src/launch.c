#include "minishell.h"

void parent_job(t_shell *this, t_node *head)
{
    t_node *tmp;

 this->last_one = head->pid;
            if (head->parent != NULL) {
                if (head->parent->op_type == pipeline) {
                    if (!head->isleft) {
                        close(head->parent->p[1]);
                        close(head->parent->p[0]);
                   }
                }
                tmp = head->parent;
                while (tmp->parent != NULL && tmp->parent->need_a_file(tmp->parent))
                    tmp = tmp->parent;
                if  (tmp->need_a_file(tmp)
                        && tmp->parent != NULL && tmp->parent->op_type == pipeline)
                {
                    close(tmp->parent->p[1]);
                    close(tmp->parent->p[0]);
                }
            }
}


t_bool  launch(t_shell *this, t_node *head)
{
    t_node *tmp;
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
        }else 
            parent_job(this, head);
    }
    return launch(this, head->left) && launch(this, head->right);
}