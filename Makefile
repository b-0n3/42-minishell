NAME=minishell
ARRAY_LIST=./lib/arraylist/libarraylist.a
ARRAY_LIST_PATH = ./lib/arraylist/

SRC = $(shell find ./src -type  f -name *.c)






all: $(NAME)

$(NAME): $(ARRAY_LIST)
	gcc $(SRC) $(ARRAY_LIST) -g -I ./headers -o $(NAME) -lreadline #-fsanitize=address

$(ARRAY_LIST):
	make -C  $(ARRAY_LIST_PATH)

clean:
	rm -rf *.o
	make fclean -C $(ARRAY_LIST_PATH)

array_list_re:
	make re -C  $(ARRAY_LIST_PATH)

fclean: clean
	rm -rf $(NAME)

re: fclean    $(NAME)