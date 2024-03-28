CFLAGS := -Wall -Wextra -Werror
CFILES := src/main.c src/utils.c src/signals.c \
          src/lexer.c src/lexer_token_func_spec.c src/lexer_token_func_word.c src/lexer_utils.c \
		  src/builtins/builtins_utils.c src/builtins/exit.c  src/builtins/cd.c src/builtins/export.c src/builtins/pwd.c src/builtins/unset.c src/builtins/env.c src/builtins/echo.c\
		  src/exec/exec.c src/exec/exec_utils.c
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
	rm -f $(LIBFT_PATH)$(LIBFT)
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re libft