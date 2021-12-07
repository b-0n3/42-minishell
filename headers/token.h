/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 02:13:46 by am-khant          #+#    #+#             */
/*   Updated: 2021/12/07 05:05:42 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H
# include "../lib/arraylist/includes/array_list.h"

typedef char			*t_string;

typedef enum e_command_type{
	enternal,
	external,
	c_none
}	t_command_type;

typedef enum e_token_type{
	word = 1,
	op = 2,
	t_none
}	t_token_type;

typedef enum e_word_type
{
	command = 0,
	args = 1,
	i_file=2,
	o_file = 3,
	w_t_none = -1
}	t_word_type;

typedef enum e_op_type
{
	pipeline=1,
	redirection=2,
	append=3,
	heredoc=4,
	input=5,
	o_t_none = -1
}	t_op_type;

typedef enum e_operation_type{
	less= '<',
	great= '>',
	_pipe= '|',
	o_none
}	t_operation_type;

typedef enum e_internal_command
{
	_pwd,
	_cd,
	_echo,
	_export,
	_unset,
	_env,
	i_c_none =-1
}	t_internal_command;

typedef struct s_node	t_node;

typedef struct s_env_ext
{
	int				l_cursor;
	int				cursor;
	int				dq;
	int				q;
	size_t			length;
	int				expand;
	int				exit_code;
	t_string		cmd;
	t_array_list	*env;
	t_string		(*next)(struct s_env_ext * this);
	t_bool			(*has_next)(struct s_env_ext * this);
}	t_env_ext;

typedef struct s_file
{
	t_string	uri;
	int			fd;
	int			exception;
	t_bool		(*open)(struct s_file *this, int mode, int perms);
	void		(*free)(struct s_file *this);
}	t_file;

typedef struct s_token{
	t_string		value;
	t_token_type	type;
	char			start_with;
	struct s_token	*(*expand)(struct s_token *this,
			int exit_code, t_array_list env);
	t_node			*(*to_node)(struct s_token *this);
	void			(*to_string)(struct s_token *this);
	t_file			*(*to_file)(struct s_token *this);
	void			(*free)(struct s_token *this);
}	t_token;

struct s_node
{
	t_string			value;
	t_array_list		args;
	t_node				*parent;
	t_node				*right;
	t_node				*left;
	pid_t				pid;
	t_bool				isleft;
	int					p[2];
	t_string			eof;
	t_word_type			word_type;
	t_file				*input_file;
	t_file				*output_file;
	t_command_type		command_type;
	t_internal_command	i_c_type;
	t_op_type			op_type;
	t_bool				(*is_operation)(t_node * this);
	t_bool				(*need_a_file)(t_node * this);
	void				(*free)(t_node *this);
	void				(*to_string)(t_node *this);
};

t_bool			file_open(t_file *this, int mode, int perms);
void			print_node(t_node *node);
t_file			*new_file(t_string uri);
t_token			*new_token(t_string value, t_token_type type);
t_op_type		find_operation_type(t_string value);
void			token_free(t_token *this);
t_string		ft_strjoin(t_string s1, t_string s2);
t_node			*new_node(void);
t_file			*token_to_file(t_token *this);
t_bool			node_is_operation(t_node *this);
t_bool			node_need_a_file(t_node *this);
void			node_free(t_node *this);
void			node_to_string(t_node *this);
t_token			*token_expand_env(t_token *this, int exit_code,
					t_array_list env);
void			new_env_ext(t_env_ext *this, t_array_list *env,
					char *cmd, int exit_code);
t_bool			env_ext_has_next(t_env_ext *this);
#endif