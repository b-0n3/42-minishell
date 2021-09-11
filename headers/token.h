#ifndef TOKEN_H
#define TOKEN_H
#include "../lib/arraylist/includes/array_list.h"

typedef enum {
    echo,
    ls,
    _export,
    external
} command_types;

// example  cat -a hell.js | grep hello  > hell.js

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

typedef struct token{
    int             order;
    command_types   type;
    t_array_list    args;
    char            *token;

};

#endif