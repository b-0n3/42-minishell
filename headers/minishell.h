/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 02:09:08 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/07 05:05:24 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "parser.h"
# include "token.h"
# include <readline/readline.h>
# include <readline/history.h>

# include <fcntl.h>
# include <sys/stat.h>
# include <unistd.h>
# include <dirent.h>

# include <fcntl.h>
# include <sys/stat.h>
# include <unistd.h>
# include <dirent.h>
# include <errno.h>
# include <unistd.h>
# include "../lib/libft/includes/libft.h"

char		*ft_itoa(int n);
void		cmd_cd(t_shell *this, t_node *head);
void		cmd_pwd(t_shell *this);
void		init_fds(t_node *node);
void		cmd_echo(t_node *node);
void		cmd_export(t_shell *this, t_node *node);
void		cmd_env(t_shell *this);
void		cmd_unset(t_shell *this, t_node *head);
void		cmd_exit(t_shell *this, t_node *head);
t_string	env_ext_next(t_env_ext *this);
t_bool		is_allowed_in_env(char ch);
void		new_env_ext(t_env_ext *this, t_array_list *env,
				char *cmd, int exit_code);
t_token		*token_expand_env(t_token *this, int exit_code, t_array_list env);

t_string	extract_env(t_env_ext *this);
t_string	extract_exit_code(t_env_ext *this);
t_string	extract_string(t_env_ext *this);
t_string	extract_after_q(t_env_ext *this);
t_string	extract_env_(t_env_ext *this);
t_string	split_env(t_string env);
t_key_map	*env_to_key_map(t_string str);
void		push_env(t_array_list *list, t_string *env);
t_bool		file_open(t_file *this, int mode, int perms);
t_bool		exec_all_heredocs(t_shell *this, t_node *head);
t_string	find_command_in_path(t_shell *this, t_node *cmd);
void		exec_heredoc(t_shell *this, t_node *head);
t_bool		inject_command(t_node *head, t_token *token);
void		add_file_heredoc(t_shell *this, t_node *head, t_token *token);
t_string	cut_w(t_env_ext *this);
void		split_it(t_array_list *this, t_string cmd, char ch);
t_token		*get_word(t_shell *this);
t_token		*get_op(t_shell *this);
t_token		*cut_token(t_shell *this);
t_token		*check_pipe(t_shell *this);
t_token		*check_less(t_shell *this);
t_token		*check_great(t_shell *this);
t_string	cut_word(t_env_ext *this);
t_bool		check_op(char ch);
t_bool		check_unclosed(t_shell *this);
t_node		*handle_word(t_shell *this, t_token *token, t_node *head);
t_bool		check_syntax(t_shell *this, t_node *pointer);
t_bool		launch(t_shell *this, t_node *head);
t_node		*handle_operator(t_shell *this, t_token *token, t_node **head);
t_exec_v	*find_function(t_shell *this, t_string value);
int			ft_isdigit(int c);
void		add_file(t_shell *this, t_node *head, t_token *token);
void		child_quit(int i);
#endif
