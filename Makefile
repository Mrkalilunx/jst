# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g -Isrc -Ithird_party/cJSON
LDFLAGS = -lm

# Source files
SRCS = src/main.c \
       src/Rjson.c \
       src/cJSON.c \
       src/config.c \
       src/clr.c

# Object files
OBJS = $(SRCS:.c=.o)

# Target executable
TARGET = jst

# Verbose output
V ?= 0
ifeq ($(V),1)
Q =
else
Q = @
endif

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "  CC    $@"
	$(Q)$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@echo "  CC    $<"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo "  CLEAN"
	$(Q)rm -f $(OBJS) $(TARGET)