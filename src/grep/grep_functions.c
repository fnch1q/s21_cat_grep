#include "grep_internals.h"

void handler(char *files[], Option o, bool many_files, int file_c, Pattern *p) {
  int regcomp_val = o.is_register_ignore ? REG_ICASE : 0;
  many_files = (file_c > 1 && !o.is_filename_ignore);

  for (int i = 0; i < file_c; i++) {
    FILE *fstream = fopen(files[i], "r");
    if (!fstream) {
      if (!o.is_file_error_ignore) {
        fprintf(stderr, "grep: %s: No such file or directory\n", files[i]);
      }
      continue;
    }
    char prefix[1000] = "";
    if (many_files) {
      sprintf(prefix, "%s:", files[i]);
    }

    if (o.is_only_filename) {
      print_filename(o, p, regcomp_val, fstream, files[i]);
    } else if (o.is_match_count) {
      print_match_c(o, prefix, p, regcomp_val, fstream);
    } else if (o.is_only_match) {
      print_matches(o, prefix, p, regcomp_val, fstream);
    } else {
      print_string(o, prefix, p, regcomp_val, fstream);
    }

    fclose(fstream);
  }
}

void print_match_c(Option o, char prefix[], Pattern *p, int regcomp_val,
                   FILE *fstream) {
  char buf[1000];
  int match_c = 0;
  while (fgets(buf, sizeof(buf), fstream)) {
    delete_new_line(buf);
    if (reg_handler(p, regcomp_val, buf, o.is_invert_results)) {
      match_c++;
    }
  }
  printf("%s%d\n", prefix, match_c);
}

void print_filename(Option o, Pattern *p, int regcomp_val, FILE *fstream,
                    char *filename) {
  char buf[1000];
  while (fgets(buf, sizeof(buf), fstream)) {
    delete_new_line(buf);
    if (reg_handler(p, regcomp_val, buf, o.is_invert_results)) {
      printf("%s\n", filename);
      break;
    }
  }
}

void print_matches(Option o, char prefix[], Pattern *p, int regcomp_val,
                   FILE *fstream) {
  char buf[1000];
  int row_num = 0;

  while (fgets(buf, sizeof(buf), fstream)) {
    delete_new_line(buf);
    if (o.is_num_row) {
      row_num++;
    }

    int start = 0;
    while (start < (int)strlen(buf)) {
      regmatch_t earliest_match = {.rm_so = -1, .rm_eo = -1};

      for (int i = 0; i < p->count; i++) {
        regex_t regex;
        if (regcomp(&regex, p->val[i], regcomp_val)) {
          fprintf(stderr, "Ошибка компиляции рег. выражения\n");
          exit(1);
        }

        regmatch_t pmatch;
        if (!regexec(&regex, buf + start, 1, &pmatch, 0)) {
          if (earliest_match.rm_so == -1 ||
              pmatch.rm_so < earliest_match.rm_so) {
            earliest_match = pmatch;
          }
        }
        regfree(&regex);
      }

      if (earliest_match.rm_so != -1 && !o.is_invert_results) {
        printf("%s", prefix);
        if (o.is_num_row) {
          printf("%d:", row_num);
        }
        printf("%.*s\n", (int)(earliest_match.rm_eo - earliest_match.rm_so),
               buf + start + earliest_match.rm_so);
        start += earliest_match.rm_eo;
      } else {
        break;
      }
    }
  }
}

void print_string(Option o, char prefix[], Pattern *p, int regcomp_val,
                  FILE *fstream) {
  char buf[1000];
  int row_num = 0;

  while (fgets(buf, sizeof(buf), fstream)) {
    delete_new_line(buf);
    if (o.is_num_row) {
      row_num++;
    }

    if (reg_handler(p, regcomp_val, buf, o.is_invert_results)) {
      printf("%s", prefix);
      if (o.is_num_row) {
        printf("%d:", row_num);
      }
      printf("%s\n", buf);
    }
  }
}

bool reg_handler(Pattern *p, int regcomp_val, char buf[],
                 bool is_invert_results) {
  bool found = false;
  for (int i = 0; i < p->count; i++) {
    regex_t regex;
    if (regcomp(&regex, p->val[i], regcomp_val)) {
      fprintf(stderr, "Ошибка компиляции рег. выражения\n");
      exit(1);
    }
    if (!regexec(&regex, buf, 0, NULL, 0)) {
      found = true;
    }
    regfree(&regex);
  }
  return is_invert_results ? !found : found;
}

void delete_new_line(char buf[]) {
  int last_symb = (int)strlen(buf) - 1;
  if (buf[last_symb] == '\n') {
    buf[last_symb] = '\0';
  }
}

void free_strings(int count, char *string[]) {
  for (int i = 0; i < count; i++) {
    free(string[i]);
  }
}
