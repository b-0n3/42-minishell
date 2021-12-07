NAME=minishell
ARRAY_LIST=./lib/arraylist/libarraylist.a
ARRAY_LIST_PATH = ./lib/arraylist/

SRC = ./src/entry_point.c \
	./src/env_extracter.c \
	./src/env_extracter_.c \
	./src/env_parse.c \
	./src/exec/cmd_cd.c \
	./src/exec/cmd_echo.c \
	./src/exec/cmd_env.c \
	./src/exec/cmd_env_utils.c \
	./src/exec/cmd_exit.c \
	./src/exec/exec_builtins.c \
	./src/exec/exec_functions.c \
	./src/exec/exec_heredoc.c \
	./src/exec/exec_init.c \
	./src/exec/exec_init_fds.c \
	./src/exec/exec_other.c \
	./src/exec/shell_execute.c \
	./src/file_add.c \
	./src/file_init.c \
	./src/launch.c \
	./src/map.c \
	./src/node.c \
	./src/operator_parse.c \
	./src/parse_word.c \
	./src/shell.c \
	./src/shell_init.c \
	./src/syntax_check.c \
	./src/token.c \
	./src/token_init.c \
	./src/tokenizer.c \
	./src/tokenizer_checks.c \
	./src/tokenizer_getters.c \
	./src/utils.c \
	./src/utils_split.c \
	./src/child_signals.c

LIBFT_DIR=./lib/libft
LIBFT=./lib/libft/libft.a
RD_INC=-I/goinfre/aait-ham/.brew/opt/readline/include
RD_LIB=-L/goinfre/aait-ham/.brew/opt/readline/lib -lreadline 



all: $(NAME)

$(NAME): $(ARRAY_LIST) $(LIBFT)
	@gcc -Wall -Wextra -Werror $(SRC) $(ARRAY_LIST) $(LIBFT) $(RD_INC) $(RD_LIB) -g  -I ./headers -o $(NAME) 

$(ARRAY_LIST):
	@make re -C  $(ARRAY_LIST_PATH)

$(LIBFT):
	make re -C $(LIBFT_DIR)
clean:
	@rm -rf *.o
	@make fclean -C $(ARRAY_LIST_PATH)



fclean: clean
	@rm -rf $(NAME)

re: fclean    $(NAME)

run: re
	@./$(NAME)
