NAME = minishell
CFLAGS = -Wall -Wextra -Werror -lreadline -g -fsanitize=address

SRC = 	main.c	\
		memo.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@cc $(CFLAGS) -o $(NAME) $(OBJ)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all