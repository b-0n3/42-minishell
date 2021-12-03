#include "minishell.h"

void	exec_echo(t_shell *this, t_node *node)
{
	printf("echo command \n");
}

void	exec_cd(t_shell *this, t_node *node)
{
	printf("cd command \n");
}

void	exec_pwd(t_shell *this, t_node *node)
{
	printf("pwd command \n");
}

void	exec_export(t_shell *this, t_node *node)
{
	printf("export command \n");
}

void	exec_unset(t_shell *this, t_node *node)
{
	printf("unset command \n");
}
