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

 typedef enum  e_word_type
 {
     command = 0,
     args = 1,
     i_file=2,
     o_file = 3,
     w_t_none = -1
 } t_word_type;
 typedef  enum  e_op_type
 {
     pipeline=1,
     redirection=2,
     append=3,
     heredoc=4,
     input=5,
     o_t_none = -1

 } t_op_type;
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
     i_c_none =-1
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


typedef struct s_file{
        t_string  name;
        int       fd;
        int       exception;
        t_bool  (*open)(struct s_file *this);
        void    (*free)(struct  s_file *this);
}           t_file;

typedef struct s_token{
    t_string value;
    t_token_type type;

    t_node *(*to_node)(struct  s_token *this);
    void (*to_string)(struct s_token *this);
    t_file *(*to_file)(struct s_token *this);// todo: add pointer to this function
    void (*free)(struct s_token *this);
}   t_token;
struct s_node
{
    t_string value;
    t_array_list  args;
    t_node         *parent;
    t_node         *right;
    t_node         *left;
    t_word_type  word_type;
    t_file        *input_file;
    t_file        *output_file;
    t_command_type command_type;
    t_internal_command i_c_type;
    t_op_type op_type;
    t_bool       (*is_operation)(t_node *this);
    t_bool       (*need_a_file)(t_node *this);
    void (*free)(t_node *this);
    void (*to_string)(t_node *this);
};

void print_node(t_node *node);
t_file *new_file(t_string uri);
t_token  *new_token(t_string value, t_token_type type);
t_op_type find_operation_type(t_string value);
void token_free(t_token *this);

//t_node  *to_node(t_token *this);
t_node  *new_node();
t_file  *token_to_file(t_token *this);
//t_bool node_add_child(t_node *this, t_node *new_node, int l_r);
t_bool node_is_operation(t_node *this);
t_bool node_need_a_file(t_node *this);
void  node_free(t_node *this);
void node_to_string(t_node *this);
#endif