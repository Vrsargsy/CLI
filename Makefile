NAME1	= client
NAME2	= server
SRCS1	= client.c get_next_line.c get_next_line_utils.c handling.c
SRCS2 = server.c handling.c
OBJS1	= $(SRCS1:.c=.o)
OBJS2	= $(SRCS2:.c=.o)
CC		= gcc
RM		= rm -f
CFLAGS	= -Wall -Wextra -Werror

.c.o :
	$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

all : $(NAME1) $(NAME2)

$(NAME1) : $(OBJS1)
	$(CC) $(CFLAGS) $(OBJS1) -o $(NAME1)

$(NAME2) : $(OBJS2)
	$(CC) $(CFLAGS) $(OBJS2) -o $(NAME2)

clean :
	$(RM) $(OBJS1) $(OBJS2)

fclean : clean
	$(RM) $(NAME1) $(NAME2)

re : fclean all

.PHONY: all clean fclean re
