#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "defs.h"

void get_separated_form(char *result, char *expression) {
  int result_loc = 0;

  int last_isalnum = 0;
  int last_isspace = 1;

  int should_place_space = 0;
  for (int i = 0; expression[i] != '\0'; i++) {
    should_place_space = (isalnum(expression[i]) && !last_isalnum)
      || (!isalnum(expression[i]) && expression[i] != ' ');

    should_place_space = should_place_space && !last_isspace;

    if (last_isspace && expression[i] == ' ') {
      continue;
    }

    if (should_place_space) {
      result[result_loc] = ' ';
      result_loc++;
      result[result_loc] = expression[i];
      result_loc++;
    }
    else {
      result[result_loc] = expression[i];
      result_loc++;
    } 
    last_isalnum = isalnum(expression[i]);
    last_isspace = expression[i] == ' ';
  }
  result[result_loc] = '\0';
}

char **token_list_from_str(char *expression) {
  char **tokens = calloc(N, sizeof(char *));
  int tokens_index = 0;

  char separated_form[N];
  get_separated_form(separated_form, expression);

  char *dup_string = strdup(separated_form);
  char *token = strsep(&dup_string, " ");
  while (token != NULL) {
    if (strcmp(token, "") != 0) {
      tokens[tokens_index++] = token;
    }
    token = strsep(&dup_string, " ");
  }
  tokens[tokens_index] = "$";
  return tokens;
}

