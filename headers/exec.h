#ifndef EXEC_H
#define EXEC_H

# include "token.h"
# define MAXPATH 4096
typedef struct s_plug{
	int in;
	int	out;
}	t_plug;

void		shell_exec(t_shell *this);
void		cmd_cd(t_shell this,t_node args, t_plug con);
void		cmd_pwd(t_shell this, t_plug con);
void		cmd_echo(t_node node, t_plug con);
t_key_map	*env_to_key_map(t_string str);
void		cmd_env(t_shell this, t_plug con);
void		cmd_unset(t_shell shell, t_string key);
void		cmd_export(t_shell this, t_node node);
int			ft_isdigit(int c);
int			arg_is_digit(char *arg);
void    	cmd_exit(t_shell this, t_node node, t_plug con);
#endif