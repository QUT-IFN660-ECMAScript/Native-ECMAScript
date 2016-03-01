%{
#include <stdio.h>
#include "y.tab.h"
}%

digit [0-9]
character [a-zA-Z]
%%
,                                   {printf("COMMA\n");}
"."                                 {printf("FULLSTOP\n");}
"("                                 {printf("LPAREN\n");}
")"                                 {printf("RPAREN\n");}
"{"                                 {printf("LBRACE\n");}
"}"                                 {printf("RBRACE\n");}
"while"|"for"|"if"|"new"            {printf("KEYWORD(%s)\n", yytext);}
"true"|"false"                      {printf("BOOLEAN(%s)\n", yytext);}
{digit}+                            {printf("INTEGER(%s)\n", yytext);}
[ \t\n]                             // do nothing
.                                   {printf("Error unknown character, %c", *yytext);}
%%
int yywrap() {
    return 1;
}

int main( int argc, char **argv )
{
    yylex();
}