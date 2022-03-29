#include <stdio.h>
#include "parser-test.h"
#include "scanner-test.h"

int main()
{
  printf("Running tests...\n");
  run_scanner_tests();
  run_parser_tests();
  printf("Completed tests!\n");
}