#include "cat_internals.h"

static void parse_short_flag(char flag, Cat_flags *flags);
static bool parse_gnu_flag(const char *arg, Cat_flags *flags);

char *parse_args(int argc, char *argv[], Cat_flags *flags, char *files[], int *file_count) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == '-') {
                if (!parse_gnu_flag(argv[i], flags)) {
                    return argv[i];
                }
            } else {
                for (int j = 1; j < (int)strlen(argv[i]); j++) {
                    parse_short_flag(argv[i][j], flags);
                }
            }
        } else {
            files[(*file_count)++] = argv[i];
        }
    }
    return NULL;
}

static void parse_short_flag(char flag, Cat_flags *flags) {
    switch (flag) {
        case 'A':
            flags->is_nonprinting_chars = true;
            flags->is_tabs = true;
            flags->is_end_of_string = true;
            break;
        case 't':
            flags->is_nonprinting_chars = true;
            flags->is_tabs = true;
            break;
        case 'T':
            flags->is_tabs = true;
            break;
        case 'e':
            flags->is_nonprinting_chars = true;
            flags->is_end_of_string = true;
            break;
        case 'E':
            flags->is_end_of_string = true;
            break;
        case 's':
            flags->is_delete_empty_line = true;
            break;
        case 'n':
            if (!flags->is_non_empty_line_numbers) {
                flags->is_all_line_numbers = true;
            }
            break;
        case 'b':
            flags->is_non_empty_line_numbers = true;
            flags->is_all_line_numbers = false;
            break;
        case 'v':
            flags->is_nonprinting_chars = true;
            break;
        default:
            break;
    }
}

static bool parse_gnu_flag(const char *arg, Cat_flags *flags) {
    if (strcmp(arg, GNU_all_line_numbers) == 0) {
        if (!flags->is_non_empty_line_numbers) {
            flags->is_all_line_numbers = true;
        }
    } else if (strcmp(arg, GNU_non_empty_line_numbers) == 0) {
        flags->is_non_empty_line_numbers = true;
        flags->is_all_line_numbers = false;
    } else if (strcmp(arg, GNU_delete_empty_line) == 0) {
        flags->is_delete_empty_line = true;
    } else {
        return false;
    }
    return true;
}