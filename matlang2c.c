#include <stdio.h>
#include "scanner.h"

int main(int argc, char *argv[])
{
   if (argc != 2)
   {

      printf("Give filename as command line argument\n");

      return (1);
   }
   Token *tokens = get_tokens_from_file(argv[1]);
}
