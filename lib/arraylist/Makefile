# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aait-ham <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/02/16 15:29:42 by aait-ham          #+#    #+#              #
#    Updated: 2021/02/16 15:29:44 by aait-ham         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libarraylist.a	
SRC = 	./srcs/array_list.c \
		./srcs/array_list_crud.c \
		./srcs/array_list_delete_if.c \
		./srcs/array_list_filter.c \
		./srcs/array_list_foreach.c \
		./srcs/array_list_operation.c \
		./srcs/array_list_pull.c \
		./srcs/array_list_sort.c \
		./srcs/array_list_clone.c

INC = ./includes/


CC = gcc	
FLAGS= -Wall -Werror -Wextra 	
PARA  = -c	



all: $(NAME)	

$(NAME): $(OBJ)
	$(CC) $(PARA)  $(SRC) -I $(INC) $(FLAGS) -g 
	ar rc $(NAME) *.o


clean:	
	rm -f *.o	

fclean: clean	
	rm -f $(NAME)	
	rm -f includes/*.*.gch	

re: fclean all
