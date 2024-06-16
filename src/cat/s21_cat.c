#include "cat_internals.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Недостаточно аргументов\n");
        return 0;
    }

    char *files[argc];

    int file_count = 0;
    Cat_flags flags = {false, false, false, false, false, false};

    char *error_parse_args = parse_args(argc, argv, &flags, files, &file_count);
    if (error_parse_args != NULL) {
        printf("Неверный ключ %s\n", error_parse_args);
        return 1;
    }

    print_files(files, &flags, file_count);

    return 0;
}