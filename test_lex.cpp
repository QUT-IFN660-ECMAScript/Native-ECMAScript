//
// Created by Freeman on 21/03/2016.
//

#include <cstdlib>
#include <FlexLexer.h>
#include "scanner.hpp"

int main(int argc, char* argv[]) {

    if (argc != 2)
        return (EXIT_FAILURE);

    yyFlexLexer* lexer = new yyFlexLexer();
    int token;
    do {
        token = lexer->yylex();
        switch (token) {
            case ECMA::Parser::token::VAR:
                printf("VAR\n");
                break;
            case ECMA::Parser::token::VALUE_INTEGER:
                printf("VALUE_INTEGER (%d)\n", atoi(lexer->YYText()));
                break;
            case ECMA::Parser::token::VALUE_FLOAT:
                printf("VALUE_FLOAT (%g)\n", atof(lexer->YYText()));
                break;
            case ECMA::Parser::token::VALUE_STRING:
                printf("VALUE_STRING (%s)\n", lexer->YYText());
                break;
            case ECMA::Parser::token::IDENTIFIER:
                printf("IDENTIFIER (%s)\n", lexer->YYText());
                break;
            case ECMA::Parser::token::RETURN:
                printf("RETURN\n");
                break;
            case ECMA::Parser::token::EXPORT:
                printf("EXPORT\n");
                break;
            case ECMA::Parser::token::CONST:
                printf("CONST\n");
                break;
            case ECMA::Parser::token::FUNCTION:
                printf("FUNCTION\n");
                break;
            case ECMA::Parser::token::ARROW_FUNCTION:
                printf("ARROW_FUNCTION\n");
                break;
// assignment
            case ECMA::Parser::token::ASSIGNMENT:
                printf("=\n");
                break;
// operators
            case ECMA::Parser::token::ADD:
                printf("+\n");
                break;
            case ECMA::Parser::token::MULTIPLY:
                printf("*\n");
                break;
            case ECMA::Parser::token::SUBTRACT:
                printf("-\n");
                break;
            case ECMA::Parser::token::DIVIDE:
                printf("/\n");
                break;
            case ECMA::Parser::token::MODULO:
                printf("MODULO\n");
                break;
            case ECMA::Parser::token::UNARY_ADD:
                printf("++\n");
                break;
            case ECMA::Parser::token::UNARY_SUBTRACT:
                printf("--\n");
                break;
// compare
            case ECMA::Parser::token::EQUAL:
                printf("==\n");
                break;
            case ECMA::Parser::token::NOT_EQUAL:
                printf("!=\n");
                break;
            case ECMA::Parser::token::EXACTLY_EQUAL:
                printf("===\n");
                break;
            case ECMA::Parser::token::NOT_EXACTLY_EQUAL:
                printf("!==\n");
                break;
            case ECMA::Parser::token::LESS_THAN:
                printf("<\n");
                break;
            case ECMA::Parser::token::GREATER_THAN:
                printf(">\n");
                break;
            case ECMA::Parser::token::LESS_THAN_OR_EQUAL:
                printf("<=\n");
                break;
            case ECMA::Parser::token::GREATER_THAN_OR_EQUAL:
                printf(">=\n");
                break;
// logical operators
            case ECMA::Parser::token::LOGICAL_AND:
                printf("&&\n");
                break;
            case ECMA::Parser::token::LOGICAL_OR:
                printf("||\n");
                break;
// bitwise operators
            case ECMA::Parser::token::LEFT_SHIFT:
                printf("<<\n");
                break;
            case ECMA::Parser::token::SIGNED_RIGHT_SHIFT:
                printf(">>\n");
                break;
            case ECMA::Parser::token::UNSIGNED_RIGHT_SHIFT:
                printf(">>>\n");
                break;
            case ECMA::Parser::token::BITWISE_AND:
                printf("&\n");
                break;
            case ECMA::Parser::token::BITWISE_OR:
                printf("|\n");
                break;
            case ECMA::Parser::token::BITWISE_XOR:
                printf("^\n");
                break;
// brackets
            case ECMA::Parser::token::LEFT_BRACKET:
                printf("[\n");
                break;
            case ECMA::Parser::token::RIGHT_BRACKET:
                printf("]\n");
                break;
            case ECMA::Parser::token::LEFT_PAREN:
                printf("(\n");
                break;
            case ECMA::Parser::token::RIGHT_PAREN:
                printf(")\n");
                break;
            case ECMA::Parser::token::LEFT_BRACE:
                printf("{\n");
                break;
            case ECMA::Parser::token::RIGHT_BRACE:
                printf("}\n");
                break;
// punctuation
            case ECMA::Parser::token::FULL_STOP:
                printf(".\n");
                break;
            case ECMA::Parser::token::COMMA:
                printf(",\n");
                break;
            case ECMA::Parser::token::SEMICOLON:
                printf(";\n");
                break;
            case ECMA::Parser::token::COLON:
                printf(":\n");
                break;
            case ECMA::Parser::token::QUESTION_MARK:
                printf("?\n");
                break;
// logical
            case ECMA::Parser::token::IF:
                printf("IF\n");
                break;
            case ECMA::Parser::token::SWITCH:
                printf("SWITCH\n");
                break;
            case ECMA::Parser::token::CASE:
                printf("CASE\n");
                break;
            case ECMA::Parser::token::DEFAULT:
                printf("DEFAULT\n");
                break;
            case ECMA::Parser::token::BREAK:
                printf("BREAK\n");
                break;
            case ECMA::Parser::token::LITERAL_TRUE:
                printf("true\n");
                break;
            case ECMA::Parser::token::LITERAL_FALSE:
                printf("false\n");
                break;
            case ECMA::Parser::token::LITERAL_NULL:
                printf("null\n");
                break;
// loops
            case ECMA::Parser::token::FOR:
                printf("FOR\n");
                break;
            case ECMA::Parser::token::DO:
                printf("DO\n");
                break;
            case ECMA::Parser::token::WHILE:
                printf("WHILE\n");
                break;
// try catch finally
            case ECMA::Parser::token::TRY:
                printf("TRY\n");
                break;
            case ECMA::Parser::token::CATCH:
                printf("CATCH\n");
                break;
            case ECMA::Parser::token::FINALLY:
                printf("FINALLY\n");
                break;

// end of file
            case ECMA::Parser::token::END_OF_FILE:
                printf("END_OF_FILE\n");
                break;
            default:
                printf("Unexpected token %d\n", token);
        }
    } while (token != ECMA::Parser::token::END_OF_FILE);

    return(EXIT_SUCCESS);

}
