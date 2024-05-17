CFLAGS := -Wall -Wextra -Werror
SRC := src/main.c 
LEXER := src/lexer/lexer.c \
		 src/lexer/lexer_token_func_spec.c \
		 src/lexer/lexer_token_func_word.c \
		 src/lexer/lexer_utils.c
PARSER := src/parser/parser.c \
		  src/parser/parser_utils.c \
		  src/parser/parser_expand.c \
		  src/parser/parser_expand_utils.c \
		  src/parser/parser_cmd.c \
		  src/parser/parser_redir.c \
		  src/parser/parser_print.c \
		  src/parser/parser_free.c
EXEC :=	src/exec/execute.c \
		src/exec/fork.c \
		src/exec/redirect.c \
		src/exec/utils.c \
		src/exec/commands.c \
		src/exec/info.c
BUILTINS := src/builtins/exit.c \
			src/builtins/cd.c \
			src/builtins/export.c \
			src/builtins/env_var_utils.c \
		    src/builtins/export_utils.c \
			src/builtins/pwd.c \
			src/builtins/unset.c \
			src/builtins/env.c \
			src/builtins/echo.c
UTILS := src/utils/utils.c \
		 src/utils/signals.c \
		 src/utils/init.c \
		 src/utils/free.c
OBJ := $(SRC:.c=.o) \
	   $(LEXER:.c=.o) \
	   $(PARSER:.c=.o) \
	   $(EXEC:.c=.o) \
	   $(BUILTINS:.c=.o) \
	   $(UTILS:.c=.o)
HEREDOC := minishell_heredoc.txt
INCLUDE := include
LIBFT_PATH := libft/
LIBFT := libft.a
NAME := minishell

all: libft $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LIBFT_PATH)$(LIBFT) -lreadline -o $(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) -I$(INCLUDE) -I$(LIBFT_PATH)$(INCLUDE) $^ -o $@ -g

libft:
	@git submodule init
	@git submodule update
	@make -C libft

clean:
	@make -C libft clean
	rm -f $(OBJ)
	rm -f $(HEREDOC)

fclean: clean
	rm -f $(LIBFT_PATH)$(LIBFT)
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean re libft
