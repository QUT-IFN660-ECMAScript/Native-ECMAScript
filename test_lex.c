//
// Created by Freeman on 21/03/2016.
//

#include <stdio.h>
#include "y.tab.h"
#include "grammar.tab.h"
#include "lex.yy.h"


int main(int argc, char* argv[])
{
    int token;
    do
    {
        token = yylex();
        switch (token)
        {
            case VAR:
                printf("VAR\n");
                break;
            case VALUE_INTEGER:
                printf("VALUE_INTEGER (%d)\n", yylval.ival);
                break;
            case VALUE_FLOAT:
                printf("VALUE_FLOAT (%g)\n", yylval.fval);
                break;
            case VALUE_STRING:
                printf("VALUE_STRING (%s)\n", yylval.sval);
                break;
            case IDENTIFIER:
                printf("IDENTIFIER (%s)\n", yylval.sval);
                break;
            case ASSIGNMENT:
                printf("=\n");
                break;
            case SEMICOLON:
                printf(";\n");
                break;
            case END_OF_FILE:
                printf("END_OF_FILE");
                break;
            default:
                printf("Unexpected token %d\n", token);
        }
    } while (token != END_OF_FILE);
}
