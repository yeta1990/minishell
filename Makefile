SRCS	= srcs/main.c srcs/utils.c srcs/lexer_setters.c srcs/list_utils.c \
		  srcs/utils_2.c srcs/utils_3.c srcs/path_operations.c \
		  srcs/ft_strtrim_full_string.c srcs/ft_split_mod.c \
		  srcs/ft_memcmp.c srcs/ft_strjoin.c srcs/ft_memmove.c \
		  srcs/cmd_arrange.c srcs/ft_putstr_fd.c srcs/test_printers.c \
		  srcs/split_quotes.c srcs/get_char_pos.c srcs/free_utils.c srcs/builtins1.c \
		  srcs/builtins2.c srcs/builtins3.c srcs/envp.c \
		  srcs/executor.c srcs/ft_files.c srcs/ft_strncmp.c \
		  srcs/error_handlers.c srcs/list_utils_2.c srcs/ft_itoa.c \
		  srcs/ft_atoi.c srcs/quote_parser.c srcs/ft_isalnum.c \
		  srcs/ft_isalpha.c srcs/ft_isdigit.c srcs/utils_4.c \
		  srcs/split_quotes_utils.c srcs/get_cmd_from_user.c \
		  srcs/quote_parser_utils.c srcs/expansor.c \
		  srcs/expansor_2.c srcs/parser.c srcs/builtin_echo.c \
		  srcs/builtin_cd.c srcs/heredoc.c srcs/error_handlers_2.c \
		  srcs/executor_helpers.c srcs/shell_level.c srcs/error_handlers_3.c

INCS	= inc/ 

CC		= gcc

OBJS	= $(SRCS:.c=.o)

CFLAGS	= -Wall -Wextra -Werror -g #-fsanitize=address

NAME	= minishell

RM		= rm -f

%.o:%.c
			$(CC) $(CFLAGS) -c $< -o $(<:.c=.o) -I$(INCS) 

$(NAME):	$(OBJS) 
			$(CC) $(CFLAGS) $(OBJS) -I$(INCS) -o $(NAME) -L/Users/$(USER)/.brew/opt/readline/lib -I/Users/$(USER)/.brew/opt/readline/include -lreadline

all:		$(NAME)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

san:		$(OBJS)
			$(CC) $(CFLAGS) $(OBJS) -I$(INCS) -L/Users/$(USER)/.brew/opt/readline/lib -I/Users/$(USER)/.brew/opt/readline/include -lreadline -o $(NAME) -g0 -O -fsanitize=address 

.PHONY:		all clean fclean re
