BUILD_DIR := build/
TARGET := $(BUILD_DIR)bingchillin
SRCS := main.c

CC := gcc
INCFLAGS := -Iinclude
CFLAGS := -Wall -Wextra -ggdb $(INCFLAGS) -fsanitize=address
LDFLAGS := -Llib -lraylib -lm

$(TARGET): $(SRCS)
	mkdir -p $(BUILD_DIR)
	$(CC) $^ $(CFLAGS) -o $@ $(LDFLAGS)

.PHONY: run debug clean release
run: $(TARGET)
	./$<

debug: $(TARGET)
	gf2 ./$<

val: $(TARGET)
	valgrind ./$<

release: $(SRCS)
	mkdir -p $(BUILD_DIR)
	$(CC) release.c $(INCFLAGS) -o $(BUILD_DIR)release $(LDFLAGS)
	./$(BUILD_DIR)release
	$(CC) $^ $(INCFLAGS) -DBUILD_RELEASE -o $(TARGET) $(LDFLAGS)


clean:
	rm $(BUILD_DIR) -rf
