//
// Created by Abdelouahad Ait hamd on 11/10/21.
//

#include "minishell.h"

t_op_type find_operation_type(t_string value)
{
    if (strcmp(value, "<<") == 0)
        return heredoc;
    if (strcmp(value, "|") == 0)
        return pipeline;
    if (strcmp(value, ">>") == 0)
        return append;
    if (strcmp(value, "<") == 0)
        return input;
    if (strcmp(value, ">") == 0)
        return redirection;
    return  o_t_none;
}
/**
 * data: {
"type":"WORD",
 "value":"ls"
}
data: {
"type":"WORD",
 "value":"-al"
}
data: {
"type":"OP",
 "value":"|"
}
data: {
"type":"WORD",
 "value":"cat"
}
data: {
"type":"WORD",
 "value":"hel"
}
data: {
"type":"OP",
 "value":">>"
}
data: {
"type":"WORD",
 "value":"g"
}
data: {
"type":"OP",
 "value":"|"
}
data: {
"type":"WORD",
 "value":"s"
}
data: {
"type":"OP",
 "value":">"
}
data: {
"type":"WORD",
 "value":"s"
}
 * */
 /// file : > fsd  < sdf  >> sdf

 // todo: convert token (aka word) to node
 /// make sure to handle
t_node *word_to_node(t_token *this, t_node *previous, t_token *next)
{
    t_node  *node;
     if (previous == NULL || previous->op_type == pipeline) {
         node = new_node();
        node->word_type = command;
        node->value = strdup(this->value);
        node->parent = previous;
        if (previous != NULL) {
            previous->right = node;
            node->parent = previous;
        }
        if (next != NULL && next->type == word)
            node->args.push(&node->args, next, sizeof(t_token));
        if (previous != NULL)
                return previous;
        return node;
     }
     if (previous->left != NULL || previous->right != NULL)
     {
         if (previous->left != NULL && previous->left->command_type == command)
             previous->left->args.push(&previous->left->args, this, sizeof (t_token));
         else if(previous->left != NULL && previous->left->command_type == command)
             previous->right->args.push(&previous->right->args, this, sizeof (t_token));
     }
     return previous;
}
// cases:   command args  | command
//          >>
t_file  *token_to_file(t_token *this) {
    if (this == NULL)
            return NULL;
    return new_file(this->value);
}
t_node *operation_to_node(t_token *this, t_node *$previous, t_token *next)
{
    t_node *node;

    node = new_node();
    node->value = strdup(this->value);
    node->op_type = find_operation_type(this->value);
    if(node->need_a_file(node) && (next != NULL && next->type == word)) {
        if (node->op_type == redirection || node->op_type == append)
            node->output_file = next->to_file(next);
        else if (node->op_type == heredoc)
            node->output_file = new_file("/tmp/b0n3_heredoc");
        else
            node->input_file = next->to_file(next);
        if ($previous != NULL && $previous->op_type != pipeline)
        {
            node->parent = $previous->parent;
            node->right = $previous;
        }

    }
    if ($previous != NULL && $previous->op_type == pipeline && node->op_type != pipeline) {
        $previous->left = node;
        node->parent = $previous;
    }
    else if ($previous != NULL && node->op_type == pipeline) {
        node->right = $previous;
        $previous->parent = node;
    }
    return node;
}

//t_node  *token_to_node(t_token *this, t_node *previous, t_token *next) {
//    if (this == NULL)
//        return NULL;
//    if (this->type == word)
//        return word_to_node(this, previous, next);
//    else if (this->type == op)
//        return operation_to_node(this, previous,next);
//    return NULL;
//}
//    if (this != NULL && this->type == word) {
//        //&& next->type == word
//        if (previous != NULL && (previous->word_type == command)) {
//            previous->args.push(&previous->args, this, sizeof(t_token));
//            return previous;
//        }
//        if (previous !=  NULL  && (previous->op_type == pipeline))
//        {
//            if(previous->right != NULL && previous->right->word_type == command)
//                previous->right->args.push(&previous->right->args, this, sizeof(t_token));
//        }
//
//        if (previous == NULL || (previous->is_operation(previous) && !previous->need_a_file(previous))) {
//            node = new_node();
//            node->value = strdup(this->value);
//            node->word_type = command;
//        } else if (previous->need_a_file(previous)) {
//            file = new_file(this->value);
//            if (previous->op_type == append || previous->op_type == redirection || previous->op_type == heredoc) {
//                previous->output_file = file;
//            } else
//                previous->input_file = file;
//            return previous;
//        }
//        if (node != NULL && previous != NULL && previous->op_type == pipeline)
//        {
//            node->parent = previous;
//            previous->right = node;
//            return previous;
//        }
//    }
//    if (this != NULL && this->type == op)
//    {
//        node = new_node();
//        node->value = strdup(this->value);
//        node->word_type = w_t_none;
//        node->op_type = find_operation_type(this->value);
//        if (previous != NULL && previous->parent != NULL && previous->op_type == pipeline)
//        {
//            previous->right = node;
//            node->parent = previous;
//        }
//        else if (previous != NULL && (node->op_type == pipeline) ) {
//                node->left = previous;
//                previous->parent = node;
//        }
//        else if ( previous != NULL && previous->op_type == pipeline)
//        {
//            previous->right = node;
//            node->parent = previous;
//        }
//        else if (node->op_type == heredoc)
//        {
//            file = new_file("/tmp/b0n3_heredoc");
//            node->output_file = file;
//        } else
//        {
//            if (previous->left == NULL)
//                previous->left = node;
//            else if (previous->right == NULL)
//                previous->right = node;
//            node->parent = previous;
//        }
//    }
////    if (node != NULL && previous != NULL  && previous->op_type == pipeline)
////    {
////        previous->right = node;
////        node->parent = previous;
////    }previous
//    return node;
//}
t_file *new_file(t_string uri)
{
    t_file *file;

    file = malloc(sizeof (t_file));
    file->name = strdup(uri);
    file->exception = 0;
    file->fd = -1;
    return  file;
}
t_node  *new_node()
{
    t_node  *new;

    new  = malloc(sizeof(t_node));
    if (new == NULL)
        return (NULL);
    new_array_list(&new->args, 2, sizeof(t_token));

    new->command_type = c_none;
    new->i_c_type =i_c_none;
    new->word_type= w_t_none;
    new->op_type =o_t_none;
    new->input_file = NULL;
    new->output_file = NULL;
    new->left = NULL;
    new->parent = NULL;
    new->right = NULL;
    new->need_a_file = &node_need_a_file;
    new->is_operation = &node_is_operation;
    new->free = &node_free;
    new->to_string = &node_to_string;
    return  (new);
}

t_bool node_is_operation(t_node *this)
{

    if (this != NULL)
    {
        return  (this->op_type != o_t_none);
    }
    return FALSE;
}


t_bool node_need_a_file(t_node *this)
{
    if (this == NULL)
        return (FALSE);
    return (this->op_type != pipeline && this->op_type != o_t_none);
}

void  node_free(t_node *this)
{
    if (this == NULL)
        return;
    node_free(this->left);
    node_free(this->right);
    free(this);
}
void node_to_string(t_node *this)
{

}