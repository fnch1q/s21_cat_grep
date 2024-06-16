#include "cat_internals.h"

void print_files(char *files[], Flags *flags, int file_count) {
    int row_count = 1;

    for (int i = 0; i < file_count; i++) {
        FILE *f = fopen(files[i], "r");
        if (f == NULL) {
            printf("%s: Нет такого файла или каталога\n", files[i]);
            continue;
        }

        char ch;
        char prev_ch;

        int empty_line_count = 0;

        while ((ch = fgetc(f)) != EOF) {
            bool tabs = false;
            if (flags->is_delete_empty_line) {
                if (prev_ch == '\n') {
                    empty_line_count++;
                } else {
                    empty_line_count = 0;
                }
                if (ch == '\n' && empty_line_count > 1) {
                    prev_ch = ch;
                    continue;
                }
            }

            if (flags->is_all_line_numbers) {
                if (prev_ch == '\n' || row_count == 1) {
                    print_line_number(&row_count);
                }
            }

            if (flags->is_non_empty_line_numbers) {
                if ((prev_ch == '\n' && ch != '\n') || row_count == 1) {
                    print_line_number(&row_count);
                }
            }

            if (flags->is_end_of_string) {
                if (ch == '\n') {
                    fputc('$', stdout);
                }
            }

            if (flags->is_tabs) {
                if (ch == '\t') {
                    fputs("^I", stdout);
                    tabs = true;
                }
            }

            if (flags->is_non_printing_chars) {
                if ((unsigned char)ch > 127) {
                    char buf[] = {ch, 0};
                    char uc;
                    for (int i = 0; i < (int)strlen(buf); i++) {
                        uc = (unsigned char)buf[i];
                        uc = uc & 127;
                        if (uc < 32) {
                            printf("M-^%c", uc + 64);
                        } else {
                            printf("M-%c", uc);
                        }
                    }
                    prev_ch = ch;
                    continue;
                } else if ((unsigned char)ch < 32 && ch != 9 && ch != 10 && ch != 12) {
                    printf("^%c", ch + 64);
                    prev_ch = ch;
                    continue;
                } else if (ch == 127) {
                    printf("^?");
                    prev_ch = ch;
                    continue;
                } else if (ch == 12) {
                    printf("^L");
                    prev_ch = ch;
                    continue;
                }
            }

            if (!tabs) {
                fputc(ch, stdout);
            }
            prev_ch = ch;
            tabs = false;
        }
        fclose(f);
    }
}

void print_line_number(int *row_count) {
    printf("%*d\t", 6, *row_count);
    (*row_count)++;
}