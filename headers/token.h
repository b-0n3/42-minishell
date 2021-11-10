#ifndef TOKEN_H
#define TOKEN_H
#include "../lib/arraylist/includes/array_list.h"

typedef char * t_string;

// example 



typedef enum e_command_type{
    enternal,
    external,
    c_none
} t_command_type;


 typedef enum e_token_type{
    word = 1,
    op = 2,
     t_none
 }  t_token_type;

 typedef enum e_operation_type{
     less= '<',
     great= '>',
     pipe= '|',
     o_none

 } t_operation_type;

 typedef enum e_internal_command {
     _pwd,
     _cd,
     _echo,
     _export,
     _unset,
     _env,
     _exit,
     i_c_none
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
 typedef  struct s_node t_node;
typedef struct s_token{
    t_string value;
    t_token_type type;

    t_node *(*to_node)(struct  s_token *this);
    void (*to_string)(struct s_token *this);
    void (*free)(struct s_token *this);
}   t_token;

struct s_node
{
    t_string value;
    t_array_list  args;
    t_array_list right_nodes;
    t_array_list left_nodes;
    t_command_type command_type;
    t_internal_command i_c_type;
    t_operation_type op_type;
    t_bool (*add_child)(t_node *this, t_node *new_node, int l_r); // @Param l_r left(1) or right (2)
    void (*free)(t_node *this);
    void (*to_string)(t_node *this);
};


t_token  *new_token(t_string value, t_token_type type);
t_node  *to_node(t_token *this);
t_node  *new_node();
t_bool node_add_child(t_node *this, t_node *new_node, int l_r);
void  node_free(t_node *this);
void node_to_string(t_node *this);
#endif