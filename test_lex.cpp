//
// Created by Freeman on 21/03/2016.
//

#include <cstdlib>
#include <FlexLexer.h>
#include "ECMA_Scanner.hpp"

int main(int argc, char* argv[]) {

    if (argc != 2)
        return (EXIT_FAILURE);

    yyFlexLexer* lexer = new yyFlexLexer();
    int token;
    do {
        token = lexer->yylex();
        switch (token) {
            case ECMA::ECMA_Parser::token::VAR:
                printf("VAR\n");
                break;
            case ECMA::ECMA_Parser::token::VALUE_INTEGER:
                printf("VALUE_INTEGER (%d)\n", atoi(lexer->YYText()));
                break;
            case ECMA::ECMA_Parser::token::VALUE_FLOAT:
                printf("VALUE_FLOAT (%g)\n", atof(lexer->YYText()));
                break;
            case ECMA::ECMA_Parser::token::VALUE_STRING:
                printf("VALUE_STRING (%s)\n", lexer->YYText());
                break;
            case ECMA::ECMA_Parser::token::IDENTIFIER:
                printf("IDENTIFIER (%s)\n", lexer->YYText());
                break;
            case ECMA::ECMA_Parser::token::RETURN:
                printf("RETURN\n");
                break;
            case ECMA::ECMA_Parser::token::EXPORT:
                printf("EXPORT\n");
                break;
            case ECMA::ECMA_Parser::token::CONST:
                printf("CONST\n");
                break;
            case ECMA::ECMA_Parser::token::FUNCTION:
                printf("FUNCTION\n");
                break;
            case ECMA::ECMA_Parser::token::ARROW_FUNCTION:
                printf("ARROW_FUNCTION\n");
                break;
// assignment
            case ECMA::ECMA_Parser::token::ASSIGNMENT:
                printf("=\n");
                break;
// operators
            case ECMA::ECMA_Parser::token::ADD:
                printf("+\n");
                break;
            case ECMA::ECMA_Parser::token::MULTIPLY:
                printf("*\n");
                break;
            case ECMA::ECMA_Parser::token::SUBTRACT:
                printf("-\n");
                break;
            case ECMA::ECMA_Parser::token::DIVIDE:
                printf("/\n");
                break;
            case ECMA::ECMA_Parser::token::MODULO:
                printf("MODULO\n");
                break;
            case ECMA::ECMA_Parser::token::UNARY_ADD:
                printf("++\n");
                break;
            case ECMA::ECMA_Parser::token::UNARY_SUBTRACT:
                printf("--\n");
                break;
// compare
            case ECMA::ECMA_Parser::token::EQUAL:
                printf("==\n");
                break;
            case ECMA::ECMA_Parser::token::NOT_EQUAL:
                printf("!=\n");
                break;
            case ECMA::ECMA_Parser::token::EXACTLY_EQUAL:
                printf("===\n");
                break;
            case ECMA::ECMA_Parser::token::NOT_EXACTLY_EQUAL:
                printf("!==\n");
                break;
            case ECMA::ECMA_Parser::token::LESS_THAN:
                printf("<\n");
                break;
            case ECMA::ECMA_Parser::token::GREATER_THAN:
                printf(">\n");
                break;
            case ECMA::ECMA_Parser::token::LESS_THAN_OR_EQUAL:
                printf("<=\n");
                break;
            case ECMA::ECMA_Parser::token::GREATER_THAN_OR_EQUAL:
                printf(">=\n");
                break;
// logical operators
            case ECMA::ECMA_Parser::token::LOGICAL_AND:
                printf("&&\n");
                break;
            case ECMA::ECMA_Parser::token::LOGICAL_OR:
                printf("||\n");
                break;
// bitwise operators
            case ECMA::ECMA_Parser::token::LEFT_SHIFT:
                printf("<<\n");
                break;
            case ECMA::ECMA_Parser::token::SIGNED_RIGHT_SHIFT:
                printf(">>\n");
                break;
            case ECMA::ECMA_Parser::token::UNSIGNED_RIGHT_SHIFT:
                printf(">>>\n");
                break;
            case ECMA::ECMA_Parser::token::BITWISE_AND:
                printf("&\n");
                break;
            case ECMA::ECMA_Parser::token::BITWISE_OR:
                printf("|\n");
                break;
            case ECMA::ECMA_Parser::token::BITWISE_XOR:
                printf("^\n");
                break;
// brackets
            case ECMA::ECMA_Parser::token::LEFT_BRACKET:
                printf("[\n");
                break;
            case ECMA::ECMA_Parser::token::RIGHT_BRACKET:
                printf("]\n");
                break;
            case ECMA::ECMA_Parser::token::LEFT_PAREN:
                printf("(\n");
                break;
            case ECMA::ECMA_Parser::token::RIGHT_PAREN:
                printf(")\n");
                break;
            case ECMA::ECMA_Parser::token::LEFT_BRACE:
                printf("{\n");
                break;
            case ECMA::ECMA_Parser::token::RIGHT_BRACE:
                printf("}\n");
                break;
// punctuation
            case ECMA::ECMA_Parser::token::FULL_STOP:
                printf(".\n");
                break;
            case ECMA::ECMA_Parser::token::COMMA:
                printf(",\n");
                break;
            case ECMA::ECMA_Parser::token::SEMICOLON:
                printf(";\n");
                break;
            case ECMA::ECMA_Parser::token::COLON:
                printf(":\n");
                break;
            case ECMA::ECMA_Parser::token::QUESTION_MARK:
                printf("?\n");
                break;
// logical
            case ECMA::ECMA_Parser::token::IF:
                printf("IF\n");
                break;
            case ECMA::ECMA_Parser::token::SWITCH:
                printf("SWITCH\n");
                break;
            case ECMA::ECMA_Parser::token::CASE:
                printf("CASE\n");
                break;
            case ECMA::ECMA_Parser::token::DEFAULT:
                printf("DEFAULT\n");
                break;
            case ECMA::ECMA_Parser::token::BREAK:
                printf("BREAK\n");
                break;
            case ECMA::ECMA_Parser::token::LITERAL_TRUE:
                printf("true\n");
                break;
            case ECMA::ECMA_Parser::token::LITERAL_FALSE:
                printf("false\n");
                break;
            case ECMA::ECMA_Parser::token::LITERAL_NULL:
                printf("null\n");
                break;
// loops
            case ECMA::ECMA_Parser::token::FOR:
                printf("FOR\n");
                break;
            case ECMA::ECMA_Parser::token::DO:
                printf("DO\n");
                break;
            case ECMA::ECMA_Parser::token::WHILE:
                printf("WHILE\n");
                break;
// try catch finally
            case ECMA::ECMA_Parser::token::TRY:
                printf("TRY\n");
                break;
            case ECMA::ECMA_Parser::token::CATCH:
                printf("CATCH\n");
                break;
            case ECMA::ECMA_Parser::token::FINALLY:
                printf("FINALLY\n");
                break;

// end of file
            case ECMA::ECMA_Parser::token::END_OF_FILE:
                printf("END_OF_FILE\n");
                break;
            default:
                printf("Unexpected token %d\n", token);
        }
    } while (token != ECMA::ECMA_Parser::token::END_OF_FILE);

    return(EXIT_SUCCESS);

}
