CC=gcc
CFLAGS=-c -Wall -Wextra -Werror -std=c11
LDFLAGS=-Wall -Wextra -Werror

EXECUTABLE=s21_grep

SOURCES=s21_grep.c grep_parser.c grep_functions.c

OBJECTS=$(SOURCES:.c=.o)

.PHONY: all clean rebuild

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

rebuild: clean all