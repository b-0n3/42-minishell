/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: am-khant <am-khant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 14:13:59 by aait-ham          #+#    #+#             */
/*   Updated: 2021/12/07 05:26:15 by am-khant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <string.h>
# include <unistd.h>
# include <stdlib.h>

# include "array_list.h"

typedef unsigned char	t_byte;

typedef struct s_list
{
	void				*content;
	struct s_list		*next;
}						t_list;

int						ft_atoi_p(char **str);
void					*ft_calloc(size_t count, size_t size);
int						ft_isalpha(int c);
int						ft_atoi(const char *str);
size_t					ft_strlen(const char *s);
char					*ft_strchr(const char *s, int c);
void					*ft_memset(void *b, int c, size_t len);
void					ft_bzero(void *s, size_t n);
void					*ft_memcpy(void *dst, const void *src, size_t n);
void					*ft_memccpy(void *dst,
							const void *src, int c, size_t n);
void					*ft_memmove(void *dst, const void *src, size_t len);
void					*ft_memchr(const void *s, int c, size_t n);
int						ft_memcmp(const void *s1, const void *s2, size_t n);
char					*ft_strdup(const char *s);
#endif
