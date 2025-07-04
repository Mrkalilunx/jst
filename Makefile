# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g -Ithird_party/cJSON -Isrc/core -Isrc/json
LDFLAGS = 

# Source directories
SRC_DIR = src
CORE_DIR = $(SRC_DIR)/core
JSON_DIR = $(SRC_DIR)/json
THIRD_PARTY_DIR = third_party/cJSON

# Source files
CORE_SRCS = $(CORE_DIR)/args.c $(CORE_DIR)/color.c $(CORE_DIR)/file.c \
            $(CORE_DIR)/tree.c $(CORE_DIR)/util.c
JSON_SRCS = $(JSON_DIR)/json.c
THIRD_PARTY_SRCS = $(THIRD_PARTY_DIR)/cJSON.c
MAIN_SRC = $(SRC_DIR)/main.c

# Object files
OBJS = $(CORE_SRCS:.c=.o) $(JSON_SRCS:.c=.o) $(THIRD_PARTY_SRCS:.c=.o) $(MAIN_SRC:.c=.o)

# Target executable
TARGET = json_tree

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)