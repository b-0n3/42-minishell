# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aait-ham <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/07 05:53:21 by aait-ham          #+#    #+#              #
#    Updated: 2021/12/07 05:53:22 by aait-ham         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=libft.a

SRC= ./srcs/ft_atoi.c \
	./srcs/ft_bzero.c \
	./srcs/ft_calloc.c \
	./srcs/ft_isalpha.c \
	./srcs/ft_memccpy.c \
	./srcs/ft_memchr.c \
	./srcs/ft_memcmp.c \
	./srcs/ft_memcpy.c \
	./srcs/ft_memmove.c \
	./srcs/ft_memset.c \
	./srcs/ft_strchr.c \
	./srcs/ft_strdup.c \
	./srcs/ft_strlen.c \
	./srcs/ft_strndup.c

OBJ=$(SRC:.c=.o)

%.o: %.c
	gcc -c -Wall -Wextra -Werror -I ./includes $< -o $@
	ar rcs $(NAME) $@

all: $(NAME)

$(NAME): $(OBJ)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
