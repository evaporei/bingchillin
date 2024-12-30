TARGET := bingchillin
SRCS := main.c

CC = gcc
CFLAGS = -Wall -Wextra -ggdb
LDFLAGS := -lraylib

.PHONY: run debug clean
run: $(TARGET)
	./$<

debug: $(TARGET)
	gf2 ./$<

$(TARGET): $(SRCS)
	gcc $^ $(CFLAGS) -o $@ $(LDFLAGS)

clean:
	rm $(TARGET)
