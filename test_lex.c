//
// Created by Freeman on 21/03/2016.
//

#include <stdio.h>
#include "y.tab.h"
#include "ast/ast.hpp"
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
            case RETURN:
                printf("RETURN\n");
                break;
            case EXPORT:
                printf("EXPORT\n");
                break;
            case CONST:
                printf("CONST\n");
                break;
            case FUNCTION:
                printf("FUNCTION\n");
                break;
            case ARROW_FUNCTION:
                printf("ARROW_FUNCTION\n");
                break;
// assignment
            case ASSIGNMENT:
                printf("=\n");
                break;
// operators
            case ADD:
              	printf("+\n");
              	break;
            case MULTIPLY:
              	printf("*\n");
              	break;
            case SUBTRACT:
              	printf("-\n");
              	break;
            case DIVIDE:
              	printf("/\n");
              	break;
            case MODULO:
                printf("MODULO\n");
                break;
            case UNARY_ADD:
                printf("++\n");
                break;
            case UNARY_SUBTRACT:
                printf("--\n");
                break;
// compare
            case EQUAL:
                printf("==\n");
                break;
            case NOT_EQUAL:
                printf("!=\n");
                break;
            case EXACTLY_EQUAL:
                printf("===\n");
                break;
            case NOT_EXACTLY_EQUAL:
                printf("!==\n");
                break;
            case LESS_THAN:
                printf("<\n");
                break;
            case GREATER_THAN:
                printf(">\n");
                break;
            case LESS_THAN_OR_EQUAL:
                printf("<=\n");
                break;
            case GREATER_THAN_OR_EQUAL:
                printf(">=\n");
                break;
// logical operators
            case LOGICAL_AND:
                printf("&&\n");
                break;
            case LOGICAL_OR:
                printf("||\n");
                break;
// bitwise operators
            case LEFT_SHIFT:
                printf("<<\n");
                break;
            case SIGNED_RIGHT_SHIFT:
                printf(">>\n");
                break;
            case UNSIGNED_RIGHT_SHIFT:
                printf(">>>\n");
                break;
            case BITWISE_AND:
                printf("&\n");
                break;
            case BITWISE_OR:
                printf("|\n");
                break;
            case BITWISE_XOR:
                printf("^\n");
                break;
// brackets
            case LEFT_BRACKET:
              	printf("[\n");
              	break;
            case RIGHT_BRACKET:
              	printf("]\n");
              	break;
            case LEFT_PAREN:
                printf("(\n");
                break;
            case RIGHT_PAREN:
                printf(")\n");
                break;
            case LEFT_BRACE:
                printf("{\n");
                break;
            case RIGHT_BRACE:
                printf("}\n");
                break;
// punctuation
            case FULL_STOP:
                printf(".\n");
                break;
            case COMMA:
                printf(",\n");
                break;
            case SEMICOLON:
                printf(";\n");
                break;
            case COLON:
                printf(":\n");
                break;
            case QUESTION_MARK:
                printf("?\n");
                break;
// logical
            case IF:
                printf("IF\n");
                break;
            case SWITCH:
                printf("SWITCH\n");
                break;
            case CASE:
                printf("CASE\n");
                break;
            case DEFAULT:
                printf("DEFAULT\n");
                break;
            case BREAK:
                printf("BREAK\n");
                break;
            case LITERAL_TRUE:
                printf("true\n");
                break;
            case LITERAL_FALSE:
                printf("false\n");
                break;
            case LITERAL_NULL:
                printf("null\n");
                break;
// loops
            case FOR:
                printf("FOR\n");
                break;
            case DO:
                printf("DO\n");
                break;
            case WHILE:
                printf("WHILE\n");
                break;
// try catch finally
            case TRY:
                printf("TRY\n");
                break;
            case CATCH:
                printf("CATCH\n");
                break;
            case FINALLY:
                printf("FINALLY\n");
                break;
			case THROW:
				printf("THROW\n");
				break;

// line terminators, print nothing
            // case LINE_FEED:
            //     break;
            // case CARRIAGE_RETURN:
            //     break;
            // case LINE_SEPARATOR:
            //     break;
            // case PARAGRAPH_SEPARATOR:
            //     break;
// end of file
            case END_OF_FILE:
                printf("END_OF_FILE\n");
                break;
            default:
                printf("Unexpected token %d\n", token);
        }
    } while (token != END_OF_FILE);
}
