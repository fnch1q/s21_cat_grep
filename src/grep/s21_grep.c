/*
сделать проверку cpp check
сделать проверку valgrind
*/
#include "grep_internals.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Недостаточно аргументов\n");
    exit(1);
  }

  char *files[argc];
  int file_c = 0;

  Pattern patterns = {NULL, 0, 0};
  Flags flags = {false, false, false, false, false,
                    false, false, false, false};
  bool many_files = false;

  char error_parse_args =
      parse_args(argc, argv, &flags, files, &file_c, &patterns);
  if (error_parse_args != 0) {
    free_strings(patterns.count, patterns.val);
    free(patterns.val);
    fprintf(stderr, "Неверный ключ %c", error_parse_args);
    exit(1);
  }

  handler(files, flags, many_files, file_c, &patterns);

  free_strings(patterns.count, patterns.val);
  free(patterns.val);

  return 0;
}