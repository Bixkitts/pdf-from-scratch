CC          = gcc
CFLAGS      = -Wall -Wextra -pedantic -std=c99
SRCS        = src/*.c
TARGET      = md_parser
BUILD_DIR   = build
DEBUG_FLAGS = -g -O0
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

clean:
	rm -f $(TARGET)

