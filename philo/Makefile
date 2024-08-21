NAME	= philo

SRCS	= main.c\
		thread.c\
		tool.c\
		philo_util.c

OBJS	= $(SRCS:.c=.o)

HEADER	= philo.h

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -fsanitize=address
INCLUDES= -I.

RM		= rm -f

all: $(NAME)

%.o: %.c $(HEADER)
	@$(CC) $(CFLAGS) -c -g $(INCLUDES) $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)
	@echo "$(NAME): object file and $(NAME) created"

clean:
	@$(RM) $(OBJS)
	@echo "$(NAME): objects deleted"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(NAME): $(NAME) deleted"

re: fclean all

.PHONY : all clean fclean re