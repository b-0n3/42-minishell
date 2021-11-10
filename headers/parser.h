 #ifndef PARSER_H
 #define PARSER_H

# include "token.h"


typedef struct s_shell
{
    t_array_list env;
    t_bool       fresh;
    char         *commmand;
    t_array_list *tokens;
    size_t command_len;
    t_array_list  *nodes;
    size_t      l_cursor;
    size_t     cursor;
    t_bool     dqout;
    t_bool     quot;
    t_bool (*unclosed)(struct s_shell *this);
    void (*init)(struct  s_shell *this, t_string line);
    t_bool (*has_next_token)(struct s_shell *this);
    void (*loop)(struct s_shell *this);
    t_token *(*get_next_token)(struct s_shell *this);
    void (*parse)(struct s_shell *this);
    void (*free)(struct s_shell *this);
}           t_shell;

void create_shell(t_shell *this, t_string *env);
void init_shell(t_shell *this, t_string line);
void shell_loop(t_shell *this);
t_token *shell_get_next_token(t_shell *this);
void shell_parse( t_shell *this);
void shell_free(t_shell *this);
t_bool has_next_token(t_shell *this);
t_bool shell_quot_unclosed(t_shell *this);
// cat f1 |  grep line >>  f1

/**
{
  "type": "Script",
  "commands": [
    {
      "type": "Pipeline",
      "commands": [
        {
          "type": "Command",
          "name": {
            "text": "cat",
            "type": "Word"
          },
          "suffix": [
            {
              "text": "f1",
              "type": "Word"
            }
          ]
        },
        {
          "type": "Command",
          "name": {
            "text": "grep",
            "type": "Word"
          },
          "suffix": [
            {
              "text": "line",
              "type": "Word"
            },
            {
              "type": "Redirect",
              "op": {
                "text": ">>",
                "type": "dgreat"
              },
              "file": {
                "text": "f",
                "type": "Word"
              }
            }
          ]
        }
      ]
    }
  ]
}
 * */


 #endif