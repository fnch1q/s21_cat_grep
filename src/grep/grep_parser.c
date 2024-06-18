#include "grep_internals.h"

char parse_args(int argc, char *argv[], Option *o, char *files[], int *file_c,
                Pattern *p) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && (int)strlen(argv[i]) > 1) {
      for (int j = 1; j < (int)strlen(argv[i]); j++) {
        char res = handle_flag(argv[i][j], argv[i][j + 1], o, p, argv[i + 1]);
        if (res != 0 && res != 'e' && res != 'f') {
          return res;
        }
        if (res == 'e') {
          o->is_pattern = true;
          char *buf = strchr(argv[i], 'e') + 1;
          add_pattern(p, buf);
          break;
        }
        if (res == 'f') {
          o->is_pattern = true;
          char *buf = strchr(argv[i], 'f') + 1;
          handle_pattern_file(buf, p);
          break;
        }
      }
      strcpy(argv[i], "");
    }
  }

  for (int k = 1; k < argc; k++) {
    if (!strcmp(argv[k], "")) {
      continue;
    }
    if (!o->is_pattern) {
      o->is_pattern = true;
      add_pattern(p, argv[k]);
    } else {
      files[(*file_c)++] = argv[k];
    }
  }
  return 0;
}

char handle_flag(char flag, char next_char, Option *o, Pattern *p,
                 char *next_arg) {
  switch (flag) {
    case 'i':
      o->is_register_ignore = true;
      break;
    case 'v':
      o->is_invert_results = true;
      break;
    case 's':
      o->is_file_error_ignore = true;
      break;
    case 'c':
      o->is_match_count = true;
      break;
    case 'n':
      o->is_num_row = true;
      break;
    case 'l':
      o->is_only_filename = true;
      break;
    case 'h':
      o->is_filename_ignore = true;
      break;
    case 'f':
      if (next_char != 0) {
        o->is_pattern = true;
        return flag;
      }
      o->is_pattern = true;
      handle_pattern_file(next_arg, p);
      strcpy(next_arg, "");
      break;
    case 'e':
      if (next_char != 0) {
        o->is_pattern = true;
        return flag;
      }
      o->is_pattern = true;
      add_pattern(p, next_arg);
      strcpy(next_arg, "");
      break;
    case 'o':
      o->is_only_match = true;
      break;
    default:
      return flag;
  }
  return 0;
}

void handle_pattern_file(const char *filename, Pattern *p) {
  FILE *fstream = fopen(filename, "r");
  if (!fstream) {
    fprintf(stderr, "grep: %s: No such file or directory\n", filename);
    free_strings(p->count, p->val);
    exit(1);
  }
  char buf[1000];

  while (fgets(buf, sizeof(buf), fstream)) {
    delete_new_line(buf);
    add_pattern(p, buf);
  }
  fclose(fstream);
}

void add_pattern(Pattern *p, const char *pattern) {
  if (p->count >= p->size) {
    p->size = p->size == 0 ? 10 : p->size * 2;
    p->val = realloc(p->val, p->size * sizeof(char *));
    if (!p->val) {
      fprintf(stderr, "Ошибка выделения памяти\n");
      exit(1);
    }
  }
  p->val[p->count] = malloc(strlen(pattern) + 1);
  if (!p->val[p->count]) {
    fprintf(stderr, "Ошибка выделения памяти\n");
    exit(1);
  }
  strcpy(p->val[p->count++], pattern);
}