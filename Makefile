NAME = minishell
CARGS = -Wall -Wextra -Werror -g3 -fsanitize=address

LIBFT = libft/libft.a

PARSING_SRC = parsing/parser.c

# EXEC_SRC = exec/

SRC =	main.c		\
		memo.c		\
		string.c	\
		error.c

SRC += $(PARSING_SRC)

#colors
YELLOW = \033[0;33m
GREEN = \033[0;32m
RED = \033[0;31m
MAGENT = \033[0;35m
BLUE = \033[0;34m
RESET = \033[0m

all: $(NAME)
build: all clean

$(NAME): $(SRC) $(LIBFT)
	@echo "$(YELLOW)Compiling minishell 🛠️"
	cc $(CARGS) $(SRC) $(LIBFT) -lreadline -o $(NAME)
	@echo "$(GREEN)Minishell compiled successfully 🚀"

$(LIBFT):
	@echo "$(MAGENT)Compiling libft 🪡"
	make -C libft

clean:
	@echo "$(BLUE)Cleaning libft 🧹"
	make -C libft clean

fclean: clean
	@echo "$(RED)Cleaning minishell 🧹"
	rm -f $(NAME)
	make -C libft fclean

re: fclean all
