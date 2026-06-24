CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
SRCS = main.c utils.c
OBJS = $(SRCS:.c=.o)
TARGET = my_program
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

fclean:
	rm -f $(OBJS) $(TARGET) *.dSYM

.PHONY: all clean fclean