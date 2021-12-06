#include "minishell.h"

extern  int mood;
static	int	check_nl(t_string item)
{
	return !strcmp(item, "-n");
}
void free_args(t_string *args)
{
    int i;

    i = 0;
    while (args[i] != NULL)
    {
        free(args[i]);
        i++;
    }
    free(args);
}


void	cmd_echo(t_node *node)
{
   t_string *args;
   int n_line;
   int i;
   args = node_to_execv_args(node);
   n_line = (args[1] != NULL && check_nl(args[1]));
   i = n_line + 1;
   while (args[i] != NULL)
   {
       write(STDOUT_FILENO, args[i], strlen(args[i]));
       if (args[i + 1] != NULL)
        write(STDOUT_FILENO, " ", 1);
       i++;
   }
   if (!n_line)
       write(STDOUT_FILENO, "\n", 1);
   free_args(args);
   if (mood == 1)
        exit(0);
}
