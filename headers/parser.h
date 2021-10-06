 #ifndef PARSER_H
 #define PARSER_H

# include "token.h"

typedef struct s_parser
{
    t_array_list tokens;
    t_array_list env;
    t_array_list pipeline;
    char         *commmand;
    void (*parse)(struct s_parser *this);

}           t_parser;




 #endif