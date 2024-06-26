#include <ctype.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef GREP_INTERNALS_H
#define GREP_INTERNALS_H

typedef struct {
  bool is_register_ignore;  // -i игнорирует регистр
  bool is_invert_results;   // -v инвертирует смысл поиска
  bool is_file_error_ignore;  // -s подавляет сообщения об ошибках
                              // несуществующего файла
  bool is_match_count;  // -c выводит количество найденных свопадений в строках
  bool is_num_row;  // -n выводит номер строки в файле
  bool is_only_filename;  // -l выводит только наименование файла
  bool is_pattern;  // есть паттерн
  bool is_filename_ignore;  // -h не выводит имена файлов если их больше 1
  bool is_only_match;  // -o печатает только совпадающие (непустые) части
                       // совпавшей строки.
} Flags;

typedef struct {
  char **val;
  int count;
  int size;
} Pattern;

void handler(char *files[], Flags flags, bool many_files, int file_c,
             Pattern *p);
void delete_new_line(char buf[]);
void print_string(Flags flags, char prefix[], Pattern *p, int regcomp_val,
                  FILE *fstream);
void print_matches(Flags flags, char prefix[], Pattern *p, int regcomp_val,
                   FILE *fstream);
void print_filename(Flags flags, Pattern *p, int regcomp_val, FILE *fstream,
                    char *filename);
void print_match_c(Flags flags, char prefix[], Pattern *p, int regcomp_val,
                   FILE *fstream);
bool reg_handler(Pattern *p, int regcomp_val, char buf[],
                 bool is_invert_results);
void free_strings(int count, char *string[]);

char parse_args(int argc, char *argv[], Flags *flags, char *files[],
                int *file_c, Pattern *p);
char handle_flag(char flag, char next_char, Flags *flags, Pattern *p,
                 char *next_arg);
void handle_pattern_file(const char *filename, Pattern *p);
void add_pattern(Pattern *p, const char *pattern);

#endif
