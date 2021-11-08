#ifndef TOKEN_H
#define TOKEN_H
#include "../lib/arraylist/includes/array_list.h"

typedef char * t_string;

// example 



typedef enum e_command_type{
    enternal,
    external
} t_command_type;


 typedef enum{
    word,
    op
 }  t_token_type;

 typedef enum{
     less='<',
     great='>',
     pipe='|'

 } e_operation_type;

 typedef enum {
     _pwd,
     _cd,
     _echo,
     _export,
     _unset,
     _env,
     _exit
 } t_internal_command;

 // cat hell > cat | grep helo | wc -l > helor $(echo dddd)


// typedef struct s_token{
//     int             order;
//     t_command_type   type;
//     token_type       token_type;
//     t_internal_command  internal_c;
//     char            *output;
//     t_array_list    args;
//     char            *token;
//     pid_t           pid;
//     t_array_list    fds;
// }               t_token;

// todo : parse line to list of tokens
/**
 * todo: create tokenizer
 */
typedef struct s_token{
    t_string value;
    t_token_type type;

    void (*to_string)(struct s_token *this);
    void (*free)(struct s_token *this);
}   t_token;

typedef struct s_node
{
    t_string value;
    t_array_list  tokens;
    t_array_list childs;
    t_command_type command_type;
    t_internal_command i_c_type;
    e_operation_type op_type;
    void (*free)(struct s_node *this);
    void (*to_string)(struct s_node *this);
}       t_node;


t_token  *new_token(t_string value, t_token_type type);
t_node  *new_node();
#endif