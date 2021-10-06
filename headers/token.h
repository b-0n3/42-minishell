#ifndef TOKEN_H
#define TOKEN_H
#include "../lib/arraylist/includes/array_list.h"


// example 

 "cat" $s -a hell.js > ls |grep hello
  word word word word operator word pipe word word 

/**
 *{
 *  "tokens":[{
 * "token": "cat",
 * "type": internal,
 * "order": 1
 * "args":[
 *      "-a",
 *      "hell.js"
 * ]},
 *  {
 *      "token":"|",
 *         type: pipe,
 *          "order": 2
 *           "args":[
 *                      {
 *                         "token":"grep"
 *                         "type": external,
 *                         "order": 1,
 *                          "args":[
 *                                  {"hello"},
 *                                  {
 *                                    "token": ">",
 *                                     "type":"redirection",
 *                                      "order":1,
 *                                      "args":[
 *                                              "hell.js"
 *                                              ]
 *                                    }
 *                                  ] 
 *                      }
 *                  ]
 * }
 *  
 * ]
 * }
 * */

typedef enum {
    enternal,
    external
} t_command_type;

typedef enum {
    _command,
    _args
} token_type;

typedef enum {
    _pwd,
    _cd, 
    _echo,
    _export,
    _unset,
    _env,
    _exit,
    none
} t_internal_command;

typedef struct token{
    int             order;
    t_command_type   type;
    token_type       token_type;
    t_internal_command  internal_c;
    char            *output;
    t_array_list    args;
    char            *token;
    pid_t           pid;
    t_array_list    fds;
};

#endif