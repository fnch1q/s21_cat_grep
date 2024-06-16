#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#ifndef CAT_INTERNALS_H
#define CAT_INTERNALS_H

// TODO флаги переделать, отдефайнить и запихать в статик массив стрингов
#define GNU_non_empty_line_numbers "--number-nonblank"
#define GNU_all_line_numbers "--number"
#define GNU_delete_empty_line "--squeeze-blank"

typedef struct {
    bool is_non_empty_line_numbers;  // нумерует непустые строки
    bool is_end_of_string;           // отображение конца строки как $
    bool is_all_line_numbers;        // нумерует все строки
    bool is_delete_empty_line;       // удаление нескольких пустых строк
    bool is_tabs;                    // отображение tab как ^|
    bool is_nonprinting_chars;       // непечатные символы
} Cat_flags;

void print_files(char *files[], Cat_flags *flags, int file_count);
void print_line_number(int *row_count);
char *parse_args(int argc, char *argv[], Cat_flags *flags, char *files[], int *file_count);

#endif
