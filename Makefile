NAME = minishell
CFLAGS = -Wall -Wextra -Werror -lreadline -g -fsanitize=address
LIBFT = libft/libft.a

SRC = 	main.c	\
		memo.c	\
		error.c	\
		parse/parser.c

OBJ = $(SRC:.c=.o)

all: $(NAME)
build: all clean

$(NAME): $(LIBFT) $(OBJ)
	@cc $(CFLAGS) $(OBJ) $(LIBFT)  -o $(NAME)

$(LIBFT):
	@make -C libft

%.o: %.c
	@cc $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@make -C libft clean

fclean: clean
	@rm -f $(NAME)
	@make -C libft fclean

re: fclean all