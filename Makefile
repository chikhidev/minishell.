NAME = minishell
CFLAGS = -Wall -Wextra -Werror -lreadline -g -fsanitize=address
LIBFT = libft/libft.a

SRC = 	main.c	\
		memo.c	\
		error.c

PARSING_SRC =	parse/parser.c	\
				parse/quotes.c	\
				parse/string.c

SRC += $(PARSING_SRC)

OBJ = $(SRC:.c=.o)

all: $(NAME)
build: all clean

$(NAME): $(LIBFT) $(OBJ)
	@echo "Compiling $(NAME)"
	@cc $(CFLAGS) $(OBJ) $(LIBFT)  -o $(NAME)

$(LIBFT):
	@echo "Compiling libft"
	@make -C libft

%.o: %.c
	@echo "Compiling $<"
	@cc $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning"
	@rm -f $(OBJ)
	@make -C libft clean

fclean: clean
	@echo "Full cleaning"
	@rm -f $(NAME)
	@make -C libft fclean

re: fclean all