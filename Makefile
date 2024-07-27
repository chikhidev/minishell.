NAME = minishell
CARGS = -Wall -Wextra -Werror -g3 -fsanitize=address -I includes
CFLAGS = -Wall -Wextra -Werror

LIBFT = libft/libft.a
GNL = get_next_line.o

PARSING_SRC = 	parsing/parser.c	\
				parsing/quoting.c	\
				parsing/quoting_utils.c	\
				parsing/paranths.c	\
				parsing/paranths2.c \
				parsing/split.c		\
				parsing/expanding.c	\
				parsing/op.c		\
				parsing/op2.c	\
				parsing/cmd.c \
				parsing/io.c

PARSING_CUSTOMED = parsing/customized/split.new.c


PARSING_SRC += $(PARSING_CUSTOMED)

EXECUTION_SRC = 	execution/exec.c \
					execution/path-checker.c \
					execution/path.c 

STRING = 	strings/string.c \
			strings/string2.c \
			strings/string3.c \
			strings/string4.c \

BUILTENS =	builtens/echo.c \
			builtens/export.c \
			builtens/env.c \
			builtens/pwd.c \
			builtens/cd.c \
	    	env_list.c	\
	    	exp_list.c	\
			builtens/index.c

SRC =	main.c		\
		memo.c		\
		error.c

SRC += $(PARSING_SRC)
SRC += $(STRING)
SRC += $(BUILTENS)
SRC += $(EXECUTION_SRC)

#colors
YELLOW = \033[0;33m
GREEN = \033[0;32m
RED = \033[0;31m
MAGENT = \033[0;35m
BLUE = \033[0;34m
RESET = \033[0m

all: $(NAME)
build: all clean

$(NAME): $(SRC) $(LIBFT) $(GNL)
	@echo "$(YELLOW)Compiling minishell 🛠️$(RESET)"
	cc $(CARGS) $(SRC) $(GNL) $(LIBFT) -lreadline -o $(NAME)
	@echo "$(GREEN)Minishell compiled successfully 🚀"

$(LIBFT):
	@echo "$(MAGENT)Compiling libft 🪡$(RESET)"
	make -C libft

$(GNL):
	@echo "$(MAGENT)Compiling get_next_line 🪡$(RESET)"
	cc $(CFLAGS) -c get_next_line/get_next_line.c

clean:
	@echo "$(BLUE)Cleaning libft 🧹$(RESET)"
	make -C libft clean
	rm -f $(GNL)

fclean: clean
	@echo "$(RED)Cleaning minishell 🧹$(RESET)"
	rm -f $(NAME)
	make -C libft fclean

re: fclean all