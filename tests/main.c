#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>

#include "../tokens-from-str.c"

static void tokens_properly_separate(void **state) {
  char expected[256][256] = { 
    "for", "(", "i", ",", "j", "in", "1", ":", "45", 
    "+", "x", ":", "1", ",", "1", ":", "4", ":", "1", ")", "{"
  };
  int num_tokens = sizeof(expected) / sizeof(char);

  char line1[256] = "for(i,j in 1:45+x:1,1:4:1){ ";
  char **tokens = token_list_from_str(line1);
  

  for (int i = 0; strcmp(tokens[i], "$") != 0; i++) {
    assert_non_null(tokens[i]);
    assert_string_equal(tokens[i], expected[i]);
  }
}

static void separated_form_is_correct(void **state) {
  char line1[256] = "for   (i,j in 1:45+x:1,1:4:1){ ";

  char line2[256] = "for ( i , j in 1 : 45 + x : 1 , 1 : 4 : 1 ) { ";

  char result[512];

  get_separated_form(result, line1);
  assert_string_equal(line2, result);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(tokens_properly_separate),
        cmocka_unit_test(separated_form_is_correct),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
