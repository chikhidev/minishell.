NAME = minishell
CARGS = -Wall -Wextra -Werror -I includes -g3 #-fsanitize=address 


LIBFT = libft/libft.a

PARSING_SRC = 	parsing/parser.c	\
				parsing/split.c		\
				parsing/op.c		\
				parsing/op2.c	\
				parsing/cmd.c \
				parsing/str_list.c \
				parsing/file_entry.c \
				parsing/syntax.c \

QUOTES = 	parsing/quotes/index.c \
		 	parsing/quotes/utils.c \
		 	parsing/quotes/utils2.c 

TOKENIZER = parsing/tokenizer/index.c	\
			parsing/tokenizer/utils.c	\
			parsing/tokenizer/utils2.c	\

IO = 		parsing/io/index.c \
			parsing/io/utils.c

EXPANDING = parsing/expanding/index.c \
			parsing/expanding/utils1.c \
			parsing/expanding/utils2.c \

PARSING_SRC += $(EXPANDING)	
PARSING_SRC += $(TOKENIZER)	
PARSING_SRC += $(IO)
PARSING_SRC += $(QUOTES)

EXECUTION_SRC = 	execution/exec.c \
					execution/path.c \
					execution/pid.c \
					execution/fd.c \
					execution/helper.c \
					execution/exec_io.c \
					execution/exec_cmd.c \
					execution/sys.c \
					execution/sys1.c 

STRING = 	strings/string.c \
			strings/string2.c \
			strings/string3.c \
			strings/string4.c \
			strings/string5.c \
			strings/string6.c \
			strings/string7.c \
			strings/string8.c \

BUILTENS =	builtens/echo.c \
			builtens/export.c \
			builtens/export_1.c \
			builtens/export_2.c \
			builtens/export_3.c \
			builtens/env.c \
			builtens/pwd.c \
			builtens/cd.c \
			builtens/unset.c \
			builtens/exit.c \
			builtens/index.c \
	    	env_list.c	\
	    	env_crud.c	\
	    	exp_list.c \
	    	env_setup.c \
	    	exp_crud.c

SRC =	main.c		\
		memo.c		\
		memo1.c		\
		error.c	\
		signals.c	\
		prompt.c

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
push:
	git add .
	git commit -m "changes"
	git push origin main

OBJ = $(SRC:.c=.o)

$(NAME): $(OBJ) $(LIBFT)
	@echo "$(YELLOW)Compiling minishell 🛠️$(RESET)"
	cc $(CARGS) $(OBJ) $(LIBFT) -lreadline -o $(NAME)
	@echo "$(GREEN)Minishell compiled successfully 🚀$(RESET)"

%.o: %.c
	cc $(CARGS) -c $< -o $@

$(LIBFT):
	@echo "$(MAGENT)Compiling libft 🪡$(RESET)"
	make -C libft

clean:
	@echo "$(BLUE)Cleaning libft 🧹$(RESET)"
	rm -f $(OBJ)
	make -C libft clean

fclean: clean
	@echo "$(RED)Cleaning minishell 🧹$(RESET)"
	rm -f $(NAME)
	make -C libft fclean

re: fclean all
