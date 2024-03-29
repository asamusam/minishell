CFLAGS := -Wall -Wextra -Werror
CFILES := src/main.c src/utils.c src/signals.c src/envp.c \
          src/lexer.c src/lexer_token_func_spec.c src/lexer_token_func_word.c src/lexer_utils.c \
		  src/parser.c src/parser_utils.c src/parser_expand.c src/parser_expand_utils.c src/parser_cmd.c \
		  src/parser_redir.c src/parser_print.c src/parser_free.c
OFILES := $(CFILES:.c=.o)
HEREDOC := minishell_heredoc.txt
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
	rm -f $(HEREDOC)

fclean: clean
	rm -f $(LIBFT_PATH)$(LIBFT)
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re libft