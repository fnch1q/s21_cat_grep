CC=gcc
CFLAGS=-c -Wall -Wextra -Werror -std=c11
LDFLAGS=-Wall -Wextra -Werror

EXECUTABLE_CAT=s21_cat
EXECUTABLE_GREP=s21_grep
CATDIR=cat/
GREPDIR=grep/

SOURCES_CAT=$(CATDIR)s21_cat.c $(CATDIR)parsing.c $(CATDIR)cat_functions.c
SOURCES_GREP=$(GREPDIR)s21_grep.c $(GREPDIR)parsing_grep.c $(GREPDIR)grep_functions.c

OBJECTS_CAT=$(SOURCES_CAT:.c=.o)
OBJECTS_GREP=$(SOURCES_GREP:.c=.o)

.PHONY: all clean rebuild

all: $(EXECUTABLE_CAT) $(EXECUTABLE_GREP)

$(EXECUTABLE_CAT): $(OBJECTS_CAT) 
	$(CC) $(LDFLAGS) $^ -o $@

$(EXECUTABLE_GREP): $(OBJECTS_GREP)
	$(CC) $(LDFLAGS) $^ -o $@

$(CATDIR)/%.o: $(CATDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

$(GREPDIR)/%.o: $(GREPDIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS_CAT) $(OBJECTS_GREP) $(EXECUTABLE_CAT) $(EXECUTABLE_GREP)

rebuild: clean all