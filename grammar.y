%{
#include <stdio.h>
#include "y.tab.h"
#include "lex.yy.h"

%}

%token END_OF_FILE
%token BREAK
%token CASE
%token CATCH
%token CLASS
%token CONST
%token CONTINUE
%token DEBUGGER
%token DEFAULT
%token DELETE
%token DO
%token ELSE
%token EXPORT
%token EXTENDS
%token FINALLY
%token FOR
%token FUNCTION
%token IF
%token IMPORT
%token IN
%token INSTANCEOF
%token LET
%token NEW
%token OF
%token RETURN
%token SUPER
%token SWITCH
%token THIS
%token THROW
%token TRY
%token TYPEOF
%token VAR
%token VOID
%token WHILE
%token WITH
%token YIELD
%token ENUM
%token AWAIT
%token IMPLEMENTS
%token INTERFACE
%token PACKAGE
%token PRIVATE
%token PROTECTED
%token PUBLIC
%token LITERAL_NULL                       // Null
%token LITERAL_TRUE                       // true
%token LITERAL_FALSE                      // false
%token LITERAL_UNDEFINED                  // undefined
%token LITERAL_NAN                        // NaN
%token UNARY_ADD                          // ++
%token UNARY_SUBTRACT                     // --
%token LOGICAL_NOT                        // !
%token MULTIPLY                           // *
%token DIVIDE                             // /
%token MODULO                             // %
%token ADD                                // +
%token SUBTRACT                           // -
%token EQUAL                              // ==
%token NOT_EQUAL                          // !=
%token EXACTLY_EQUAL                      // ===
%token NOT_EXACTLY_EQUAL                  // !==
%token LEFT_SHIFT                         // <<
%token SIGNED_RIGHT_SHIFT                 // >>
%token UNSIGNED_RIGHT_SHIFT               // >>>
%token LESS_THAN                          // <
%token GREATER_THAN                       // >
%token LESS_THAN_OR_EQUAL                 // <=
%token GREATER_THAN_OR_EQUAL              // >=
%token QUESTION_MARK                      // ?
%token COLON                              // :
%token LOGICAL_AND                        // &&
%token LOGICAL_OR                         // ||
%token BITWISE_AND                        // &
%token BITWISE_OR                         // |
%token BITWISE_XOR                        // ^
%token BITWISE_NOT                        // ~
%token ASSIGNMENT                         // =
%token ADDITION_ASSIGNMENT                // +=
%token SUBTRACTION_ASSIGNMENT             // -=
%token MULTIPLICATION_ASSIGNMENT          // *=
%token DIVISION_ASSIGNMENT                // /=
%token MODULUS_ASSIGNMENT                 // %=
%token EXPONENTIATION_ASSIGNMENT          // **=
%token LEFT_SHIFT_ASSIGNMENT              // <<=
%token SIGNED_RIGHT_SHIFT_ASSIGNMENT      // >>=
%token UNSIGNED_RIGHT_SHIFT_ASSIGNMENT    // >>>=
%token BITWISE_AND_ASSIGNMENT             // &=
%token BITWISE_XOR_ASSIGNMENT             // ^=
%token BITWISE_OR_ASSIGNMENT              // |=
%token ARROW_FUNCTION                     // =>
%token RIGHT_PAREN                        // )
%token LEFT_PAREN                         // (
%token RIGHT_BRACE                        // }
%token LEFT_BRACE                         // {
%token RIGHT_BRACKET                      // ]
%token LEFT_BRACKET                       // [
%token COMMA                              // ,
%token FULL_STOP                          // .
%token ELLIPSIS                           // ...
%token SEMICOLON                          // ;
%token DOUBLE_QUOTE                       // "
%token SINGLE_QUOTE                       // '
%token VALUE_INTEGER
%token VALUE_FLOAT
%token VALUE_STRING
%token IDENTIFIER


%union {
    int ival;
    double fval;
    char* sval;
}

%error-verbose

%%

Program:
    SourceElements
    ;

SourceElements:
    SourceElement
    | SourceElement SourceElements
    ;

SourceElement:
    Statement
    ;

Statement:
    VariableStatement
    ;

VariableStatement:
    VAR VariableDeclarationList
    ;

VariableDeclarationList:
    VariableDeclaration
    | VariableDeclaration COMMA VariableDeclaration
    ;

VariableDeclaration:
    IDENTIFIER Initialiser
    ;

Initialiser:
  ASSIGNMENT VALUE_INTEGER								
  ;

%%
