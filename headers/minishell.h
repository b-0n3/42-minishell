/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 07:48:17 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 12:12:27 by am-khant         ###   ########.fr       */
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

char				*ft_itoa(int n);
void				cmd_cd(t_shell *this, t_node *head);
void				cmd_pwd(t_shell *this);
int					check_nl(void *item);
void				cmd_echo(t_shell *this, t_node *head);
void				cmd_export(t_shell *this, t_node *head);
void				cmd_unset(t_shell *shell, t_node *head);
void				cmd_env(t_shell *this, t_node *head);
void				exec_echo(t_shell *this, t_node *node);
void				exec_cd(t_shell *this, t_node *node);
void				exec_pwd(t_shell *this, t_node *node);
void				exec_export(t_shell *this, t_node *node);
void				exec_unset(t_shell *this, t_node *node);
void				exec_env(t_shell *this, t_node *node);
void				exec_exit(t_shell *this, t_node *node);
t_string			find_command_in_path(t_shell *this, t_node *cmd);
t_string			*node_to_execv_args(t_node *node);
void				exec_other(t_shell *this, t_node *node);
void				init_exec_builtins(t_shell *this);
t_key_map			*new_func_map(t_string key, t_exec_v *func);
t_op_type			find_operation_type(t_string value);
t_file				*token_to_file(t_token *this);
t_file				*new_file(t_string uri);
t_node				*new_node(void);
t_bool				node_is_operation(t_node *this);
t_bool				node_need_a_file(t_node *this);
void				node_free(t_node *this);
void				node_to_string(t_node *this);

t_string			split_env(t_string env);
t_key_map			*env_to_key_map(t_string str);
void				push_env(t_array_list *list, t_string *env);
void				create_shell(t_shell *this, t_string *env);
void				init_shell(t_shell *this, t_string line);

t_bool				has_next_token(t_shell *this);
char				*join_command(t_string origin, t_string next);
t_bool				shell_quot_unclosed(t_shell *this);
void				print_token(void *item);
t_token				*pre_get_next_token(t_shell *this);
t_bool				file_open(t_file *this, int mode, int perms);
void				split_it(t_array_list *this, t_string cmd, char ch);
t_array_iterator	*split(t_string cmd, char ch);
void				*str_to_token(void *str);
t_node				*token_to_node(t_token *this);
t_bool				inject_command(t_node *head, t_token *token);
void				add_file(t_shell *this, t_node *head, t_token *token);
t_node				*handle_operator(t_shell *this,
						t_token *token, t_node *head);
t_node				*handle_word(t_shell *this, t_token *token, t_node *head);
t_bool				check_syntax(t_shell *this, t_node *pointer);
t_bool				shell_parse(t_shell *this);
void				print_node(t_node *node);
t_exec_v				*find_function(t_shell *this, t_string value);
void				exec_heredoc(t_shell *this, t_node *head);
t_bool				launch(t_shell *this, t_node *head);
t_bool				close_fds(t_shell *this, t_node *head);
t_bool				wait_for_all(t_shell *this, t_node *head);
t_bool				is_built_in(t_string n);
void				shell_execute(t_shell *this);
void				*map_to_string(void *item);
t_string			*shell_env_to_arr(t_shell *this);
void				shell_loop(t_shell *this);
void				shell_free(t_shell *this);
t_token				*new_token(t_string value, t_token_type type);
t_token				*check_pipe(t_shell *this);
t_token				*check_less(t_shell *this);
t_token				*check_great(t_shell *this);
t_token				*get_word(t_shell *this);
t_token				*get_op(t_shell *this);
t_token				*cut_token(t_shell *this);
t_bool				check_op(char ch);
t_bool				check_unclosed(t_shell *this);
void				token_free(t_token *this);
t_bool				is_allowed_in_env(char ch);
t_string			cut_word(t_env_ext *this);
t_bool				is_digit(char ch);
t_string			ft_strjoin(t_string s1, t_string s2);
t_bool				env_ext_has_next(t_env_ext *this);
t_string			env_ext_next(t_env_ext *this);
void				new_env_ext(t_env_ext *this, t_array_list *env,
						char *cmd, int exit_code);
t_token				*token_expand_env(t_token *this,
						int exit_code, t_array_list env);
t_token				*shell_get_next_token(t_shell *this);					
#endif
