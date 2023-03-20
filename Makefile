NAME1	= server_
NAME2	= client_
CLIENT_SRCS = $(wildcard ./CLIENT/*.c HELPERS/*.c)
SERVER_SRCS	= $(wildcard ./SERVER/*.c HELPERS/*.c)
SERVER_OBJS	= $(SERVER_SRCS:.c=.o)
CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)
CC		= gcc
RM		= rm -f
CFLAGS	= -Wall -Wextra -Werror 

.c.o :
	$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

all : $(NAME1) $(NAME2)

$(NAME1) : $(SERVER_OBJS)
	$(CC) $(CFLAGS) $(SERVER_OBJS) -o $(NAME1)

$(NAME2) : $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -lreadline $(CLIENT_OBJS) -o $(NAME2)

clean :
	$(RM) $(CLIENT_OBJS) $(SERVER_OBJS)

fclean : clean
	$(RM) $(NAME1) $(NAME2)

re : fclean all

.PHONY: all clean fclean re server client 