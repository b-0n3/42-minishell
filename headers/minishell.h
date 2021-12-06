

#ifndef MINISHELL_H
#define MINISHELL_H
#include "parser.h"
#include "token.h"
#include <readline/readline.h>
#include <readline/history.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
char			*ft_itoa(int n);
void	cmd_cd(t_shell *this, t_node *head);
void	cmd_pwd(t_shell *this);
void    init_fds(t_node *node);
void	cmd_echo(t_node *node);
void	cmd_export(t_shell *this, t_node *node);
void	cmd_env(t_shell *this);
void	cmd_unset(t_shell *this,t_node *head);
void	cmd_exit(t_shell *this, t_node *head);
#endif
