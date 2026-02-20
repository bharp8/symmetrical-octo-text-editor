CC = gcc
CFLAGS = -Wall -Wextra -g -MMD -MP -I$(INC_DIR)
TARGET = bvim
SRCS = main.c draw.c editorConfig.c rawMode.c rows.c fileIO.c process.c errors.c
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)
INC_DIR = include

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLGAGS) -I$(INC_DIR) -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(TARGET)
