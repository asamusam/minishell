CFLAGS := -Wall -Wextra -Werror
# CFILES := src/main.c src/utils.c src/signals.c 
CFILES := src/lexer.c src/lexer_spec_token_func.c src/lexer_token_func_utils.c src/lexer_word_token_func.c
OFILES := $(CFILES:.c=.o)
INCLUDE := include
LIBFT_PATH := libft/
LIBFT := libft.a
NAME := minishell

all: libft $(NAME)

$(NAME): $(OFILES)
	$(CC) $(OFILES) $(LIBFT_PATH)$(LIBFT) -lreadline -o $(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) -I$(INCLUDE) -I$(LIBFT_PATH)$(INCLUDE) $^ -o $@ -g

libft:
	@make -C libft

clean:
	@make -C libft clean
	rm -f $(OFILES)

fclean: clean
	rm -f $(LIBFT)
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re libft