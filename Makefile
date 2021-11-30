SRCS	= srcs/main.c srcs/utils.c srcs/lexer_setters.c srcs/list_utils.c \
		  srcs/utils_2.c srcs/utils_3.c srcs/path_operations.c \
		  srcs/ft_strtrim_full_string.c srcs/ft_split_mod.c \
		  srcs/ft_memcmp.c srcs/ft_strjoin.c srcs/ft_memmove.c \
		  srcs/cmd_arrange.c srcs/ft_putstr_fd.c srcs/test_printers.c

INCS	= inc

CC		= gcc

OBJS	= $(SRCS:.c=.o)

CFLAGS	= -Wall -Wextra -Werror

NAME	= minishell

RM		= rm -f

%.o:%.c
			$(CC) $(CFLAGS) -c $< -o $(<:.c=.o) -I$(INCS)

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) $(OBJS) -I$(INCS) -lreadline -o $(NAME) 
all:		$(NAME)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

san:		$(OBJS)
			$(CC) $(CFLAGS) $(OBJS) -I$(INCS) -lreadline -o $(NAME) -fsanitize=address

.PHONY:		all clean fclean re
