#ifndef EXEC_H
#define EXEC_H

# include "token.h"
# define MAXPATH 4096
void	shell_exec(t_shell *this);
void	cmd_cd(t_shell this,t_node args, int in, int out);
void	cmd_pwd(t_shell this,int in, int out);
void	cmd_echo(t_node node, int in, int out);
void	cmd_env(t_shell this,int in, int out);
void	cmd_unset(t_shell shell,t_string key);
#endif