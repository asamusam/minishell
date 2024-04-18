CFLAGS := -Wall -Wextra -Werror
CFILES := src/main.c src/utils.c src/signals.c src/init.c src/free.c \
          src/lexer.c src/lexer_token_func_spec.c src/lexer_token_func_word.c src/lexer_utils.c \
		  src/parser.c src/parser_utils.c src/parser_expand.c src/parser_expand_utils.c src/parser_cmd.c \
		  src/parser_redir.c src/parser_print.c src/parser_free.c \
		  src/builtins/exit.c  src/builtins/cd.c src/builtins/export.c src/builtins/env_var_utils.c\
		  src/builtins/export_utils.c src/builtins/pwd.c src/builtins/unset.c src/builtins/env.c src/builtins/echo.c \
		  src/exec/exec.c src/exec/exec_utils.c src/exec/exec_cmd_utils.c src/exec/redirect.c src/exec/exec_bltin_utils.c src/info/info.c
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
