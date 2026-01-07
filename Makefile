# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g -Ithird_party/cJSON -Isrc/include
LDFLAGS = 

# Source directories
SRC_DIR = src
THIRD_PARTY_DIR = third_party/cJSON

# Source files
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/args.c \
       $(SRC_DIR)/color.c \
       $(SRC_DIR)/file.c \
       $(SRC_DIR)/tree.c \
       $(SRC_DIR)/util.c \
       $(SRC_DIR)/json.c \
       $(THIRD_PARTY_DIR)/cJSON.c

# Object files
OBJS = $(SRCS:.c=.o)

# Target executable
TARGET = jst

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)