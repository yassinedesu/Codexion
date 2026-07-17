CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
SRCS = src/main.c src/init.c src/cleanup.c src/coder.c src/dongle.c src/heap.c src/log.c src/monitor.c src/parsing.c src/sched.c src/time.c
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(DEPS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re