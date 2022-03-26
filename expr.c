#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKENS   100 
#define TOKENLENGTH 20 
#define N  1000 

char tokens[MAXTOKENS][TOKENLENGTH] ;
char str[N] ; 
int  cur = 0 ; 

int  expr(char *) ;
int  term(char *) ;
int  moreterms(char *) ;
int  factor(char *) ;
int  morefactors(char *) ;
int  is_integer(char *) ; 

void parse(char **tokens, int numtokens, char *parsed_expr)
{
   char    str[N]   ; 
   
   // read the tokens 
   numtokens = 0 ; 
   while (scanf("%s",tokens[numtokens]) != EOF) {
      numtokens++ ; 
   } 
   sprintf(tokens[numtokens],"$") ;
   numtokens++ ; 
   
   // parse the expression 
    expr(str) ; 
   strcpy(parsed_expr, str);
  
   return(0) ; 
}

int expr(char *str)
{
   char str1[N], str2[N] ; 
   
   str1[0] = str2[0] = '\0' ; 
   if (!  term(str1)) {
      return(0) ; 
   } 
   if (!  moreterms(str2)) {
      return(0) ; 
   }
   strcat(str1,str2) ; 
   strcpy(str,str1) ; 
   return(1) ;  
}

int term(char *str)
{
   char str1[N], str2[N] ; 
   
   str1[0] = str2[0] = '\0' ; 
   if (!  factor(str1)) {
      return(0) ; 
   } 
   if (!  morefactors(str2)) {
      return(0) ; 
   }

   strcat(str1,str2) ; 
   strcpy(str,str1) ; 
   return(1) ;  
}

int moreterms(char *str)
{
   char str1[N], str2[N], str3[N] ; 
   
   str1[0] = str2[0] = str3[0] = '\0' ; 
   
   if ( (strcmp(tokens[cur],"+") == 0 ) || (strcmp(tokens[cur],"-") == 0 ) ) {
       strcpy(str1,tokens[cur]) ; 
       strcat(str1," ") ; 
       cur++ ;  
       if (!  term(str2)) {
          return(0) ; 
       } 
       if (!  moreterms(str3)) {
         return(0) ; 
       }
   } 
   strcat(str2,str3) ; 
   strcat(str2,str1) ; 
   strcpy(str,str2) ; 
   return(1) ;  
}

int factor(char *str)
{
   char str1[N] ; 
   
   str1[0] = '\0' ; 

    printf("%s\n",tokens[cur]) ; 
    if ( is_integer(tokens[cur])  ) {
       strcpy(str,tokens[cur]) ; 
       strcat(str," ") ; 
       cur++ ; 
       return(1) ; 
    } 
    if ( strcmp(tokens[cur],"(") == 0 ) {
       cur++ ; 
       if ( ! expr(str1) ) {
          return(0) ;    
       }
       if ( tokens[cur] != ")" ) { 
          printf("Error: expecting paranthesis\n") ; 
          return(0) ;
       }
       cur++ ; 
       strcpy(str,str1) ; 
       return(1) ; 
    }
    printf("Error: expecting factor\n") ; 
    return(0) ; 
}


int morefactors(char *str)
{
   char str1[N], str2[N], str3[N] ; 
   
   str1[0] = str2[0] = str3[0] = '\0' ; 
   
   if ( (strcmp(tokens[cur],"*") == 0 ) || (strcmp(tokens[cur],"/") == 0 ) ) {
       strcpy(str1,tokens[cur]) ; 
       strcat(str1," ") ; 
       cur++ ;  
       if (!  factor(str2)) {
          return(0) ; 
       } 
       if (!  morefactors(str3)) {
         return(0) ; 
       }
   } 
   strcat(str2,str3) ; 
   strcat(str2,str1) ; 
   strcpy(str,str2) ; 
   return(1) ;  
}


int is_integer(char *token) 
{
    int isnumber = 1 ;
    char *q ; 

    for(q = token ; *q != '\0' ; q++) {
        isnumber = isnumber && isdigit(*q) ;
    }

    return(isnumber) ; 
}    

 

