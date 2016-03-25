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
            case IDENTIFIER:
                printf("IDENTIFIER (%s)\n", yylval.sval);
                break;
            case ASSIGNMENT:
                printf("=\n");
                break;
            case SEMICOLON:
                printf(";\n");
                break;
            case IF:
                printf("IF\n");
                break;
            case EQUAL:
                printf("==\n");
                break;
            case LEFT_PAREN:
                printf("(\n");
                break;
            case RIGHT_PAREN:
                printf(")\n");
                break;
            case LITERAL_TRUE:
                printf("true\n");
                break;
            case LITERAL_FALSE:
                printf("false\n");
                break;
            case END_OF_FILE:
                printf("END_OF_FILE");
                break;
            default:
                printf("Unexpected token %d\n", token);
        }
    } while (token != END_OF_FILE);
}