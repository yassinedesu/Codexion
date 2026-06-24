CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
SRCS = main.c PARSER/parsing.c MONITOR/monitor.c utils.c 
OBJS = $(SRCS:.c=.o)
TARGET = codexion
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

fclean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean fclean