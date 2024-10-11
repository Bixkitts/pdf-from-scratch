CC          = gcc
CFLAGS      = -mavx2 -Wall -Wextra -Wpedantic -Wno-unused-parameter -std=c2x
SRCS        = src/*.c
BUILD_DIR   = build
TARGET      = $(BUILD_DIR)/md_parser
DEBUG_FLAGS = -g -O0 -DENABLE_TEST_RUNS -DSOFT_FAIL
PROD_FLAGS  = -O2

# Targets
.PHONY: all debug prod clean

all: debug

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

prod: CFLAGS += $(PROD_FLAGS)
prod: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -f $(TARGET)

