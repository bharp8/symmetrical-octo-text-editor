CC = gcc
CFLAGS = -Wall -Wextra -g -MMD -MP
TARGET = bvim
SRCS = main.c draw.c editorConfig.c rawMode.c rows.c fileIO.c process.c errors.c
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLGAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(TARGET)
