#include <stdio.h>

int main (int argc, char *argv[]) {
   FILE *fp;

   /* Open file for reading Filename is given on the command line */

   if (argc != 2) {

     printf("Give filename as command line argument\n") ;

     return(1);

   }

   fp = fopen(argv[1], "r");

   if(fp == NULL) {

      printf("Cannot open %s\n",argv[1]);

      return(1);

   }
   char line[256];

   while (fgets(line,256,fp) != NULL) {
      printf("%s",line) ;
   }

   fclose(fp);
   return(0);

}
