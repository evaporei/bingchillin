TARGET := bingchillin
SRCS := main.c

CC = gcc
CFLAGS = -Wall -Wextra -ggdb
LDFLAGS := -lraylib

$(TARGET): $(SRCS)
	gcc $^ $(CFLAGS) -o $@ $(LDFLAGS)

.PHONY: run debug clean
run: $(TARGET)
	./$<

debug: $(TARGET)
	gf2 ./$<

val: $(TARGET)
	valgrind ./$<

clean:
	rm $(TARGET)
