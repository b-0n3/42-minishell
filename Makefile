NAME=minishell
ARRAY_LIST=./lib/arraylist/libarraylist.a
ARRAY_LIST_PATH = ./lib/arraylist/

SRC = $(shell find ./src -type  f -name *.c)
LIBFT_DIR=./lib/libft
LIBFT=./lib/libft/libft.a
RD_INC=-I/goinfre/aait-ham/.brew/opt/readline/include
RD_LIB=-L/goinfre/aait-ham/.brew/opt/readline/lib -lreadline 



all: $(NAME)

$(NAME): $(ARRAY_LIST) $(LIBFT)
	@gcc $(SRC) $(ARRAY_LIST) $(LIBFT) $(RD_INC) $(RD_LIB) -g  -I ./headers -o $(NAME) 

$(ARRAY_LIST):
	@make -C  $(ARRAY_LIST_PATH)

$(LIBFT):
	make re -C $(LIBFT_DIR)
clean:
	@rm -rf *.o
	@make fclean -C $(ARRAY_LIST_PATH)

array_list_re:
	@make re -C  $(ARRAY_LIST_PATH)

fclean: clean
	@rm -rf $(NAME)

re: fclean    $(NAME)

run: re
	@./$(NAME)
