/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 12:07:51 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/06 12:12:48 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "token.h"

typedef struct s_shell	t_shell;

typedef void			t_exec_v(t_shell *this, t_node *node);

typedef struct s_func_map
{
	t_string	key;
	t_exec_v	*func;
}	t_func_map;

struct s_shell
{
	t_array_list	env;
	t_array_list	exec_pool;
	t_bool			fresh;
	t_string		commmand;
	t_string		parsing_error;
	t_array_list	*built_ins;
	size_t			command_len;
	pid_t			last_one;
	t_node			*head;
	int				exit_code;
	t_array_list	*nodes;
	size_t			l_cursor;
	size_t			cursor;
	t_bool			dqout;
	t_bool			quot;
	t_string		*(*env_to_arr)(struct s_shell *this);
	t_bool			(*unclosed)(struct s_shell * this);
	void			(*init)(struct s_shell *this, t_string line);
	t_bool			(*has_next_token)(struct s_shell * this);
	void			(*loop)(struct s_shell *this);
	t_token			*(*get_next_token)(struct s_shell *this);
	void			(*execute)(struct s_shell *this);
	t_bool			(*parse)(struct s_shell * this);
	void			(*free)(struct s_shell *this);
};

t_string			*shell_env_to_arr(t_shell *this);
void				create_shell(t_shell *this, t_string *env);
void				init_shell(t_shell *this, t_string line);
void				shell_loop(t_shell *this);
t_token				*shell_get_next_token(t_shell *this);
t_bool				shell_parse( t_shell *this);
void				shell_free(t_shell *this);
t_bool				has_next_token(t_shell *this);
t_bool				shell_quot_unclosed(t_shell *this);
t_key_map			*new_func_map(t_string key, t_exec_v *func);
void				shell_execute(t_shell *this);
void				exec_echo(t_shell *this, t_node *node);
void				exec_cd(t_shell *this, t_node *node);
void				exec_pwd(t_shell *this, t_node *node);
void				exec_export(t_shell *this, t_node *node);
void				exec_unset(t_shell *this, t_node *node);
void				exec_env(t_shell *this, t_node *node);
void				exec_exit(t_shell *this, t_node *node);
void				exec_other(t_shell *this, t_node *node);
void				init_exec_builtins(t_shell *this);
t_string			*node_to_execv_args(t_node *node);
t_array_iterator	*split(t_string cmd, char ch);

t_node				*token_to_node(t_token *this);
#endif