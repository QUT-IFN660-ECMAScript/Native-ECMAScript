%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {ECMA}
%define parser_class_name {ECMA_Parser}

%code requires{
   namespace ECMA {
      class ECMA_Driver;
      class ECMA_Scanner;
   }
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif
}

%parse-param { ECMA_Scanner  &scanner  }
%parse-param { ECMA_Driver  &driver  }

%code{
    #include <iostream>
    #include <cstdlib>
    #include <fstream>

    #include "ECMA_Driver.hpp"

    #undef yylex
    #define yylex scanner.yylex

}

%define parse.assert

%token END_OF_FILE 0 "end of file"
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
%token IDENTIFIER
%token <int> VALUE_INTEGER
%token <float> VALUE_FLOAT
%token <std::string> VALUE_STRING

%union {
    struct {
        int ival;
        double fval;
    };
    std::string* sval;
}


%locations

%error-verbose

%nonassoc ORDER_ELSE
%nonassoc ELSE

%nonassoc EQUAL
%nonassoc NOT_EQUAL
%nonassoc EXACTLY_EQUAL
%nonassoc NOT_EXACTLY_EQUAL

%%

Script:
    ScriptBody
    ;

ScriptBody:
    StatementList
    ;

StatementList:
    StatementListItem
    | StatementList StatementListItem
    ;
    
StatementListOptional:
    StatementList
    | 
    ;

StatementListItem:
    Statement
    | Declaration
    ;

Declaration:
    "temp"
    /* TODO The below are not implemented yet, see: section 13 of spec for implementation details
    HoistableDeclaration
    | ClassDeclaration
    | LexicalDeclaration
    */
    ;


Statement:
    BlockStatement
    | VariableStatement
    | EmptyStatement
    | IfStatement
    | BreakableStatement
    ;

BlockStatement:
    Block
    ;
    
Block:
    LEFT_BRACE StatementList RIGHT_BRACE
    | LEFT_BRACE RIGHT_BRACE
    ;

VariableStatement:
    VAR VariableDeclarationList
    ;

VariableDeclarationList:
    VariableDeclaration
    | VariableDeclarationList COMMA VariableDeclaration
    ;

VariableDeclaration:
    IDENTIFIER Initialiser
    /* TODO temp rules above, the below needs to be implemented see 13.3.2 in spec
    BindingIdentifier InitialiserOptional
    | BindingPattern Initialiser
    */
    ;

Initialiser:
  ASSIGNMENT VALUE_INTEGER
  | ASSIGNMENT VALUE_FLOAT
  | ASSIGNMENT VALUE_STRING
  /* TODO above rules are temp, below need to be implemented see 12.2.6 in spec
  ASSIGNMENT AssignmentExpression
  */
  ;

EmptyStatement:
    SEMICOLON
    ;

IfStatement:
    IF LEFT_PAREN Expression RIGHT_PAREN Statement %prec ORDER_ELSE
    | IF LEFT_PAREN Expression RIGHT_PAREN Statement ELSE Statement
    ;

BreakableStatement:
    IterationStatement
    | SwitchStatement
    ;
    
IterationStatement:
    // TODO Missing look-ahead checks, see 13.7 for more details
    DO Statement WHILE LEFT_PAREN Expression RIGHT_PAREN SEMICOLON
    | WHILE LEFT_PAREN Expression RIGHT_PAREN Statement
    | FOR LEFT_PAREN ExpressionOptional SEMICOLON ExpressionOptional SEMICOLON ExpressionOptional RIGHT_PAREN Statement
    | FOR LEFT_PAREN VAR VariableDeclarationList SEMICOLON ExpressionOptional SEMICOLON ExpressionOptional RIGHT_PAREN Statement
    | FOR LEFT_PAREN LexicalDeclaration ExpressionOptional SEMICOLON ExpressionOptional RIGHT_PAREN Statement
    | FOR LEFT_PAREN LeftHandSideExpression IN Expression RIGHT_PAREN Statement
    | FOR LEFT_PAREN VAR ForBinding IN Expression RIGHT_PAREN Statement
    | FOR LEFT_PAREN ForDeclaration IN Expression RIGHT_PAREN Statement
    | FOR LEFT_PAREN LeftHandSideExpression OF AssignmentExpression RIGHT_PAREN Statement
    | FOR LEFT_PAREN VAR ForBinding OF AssignmentExpression RIGHT_PAREN Statement
    | FOR LEFT_PAREN ForDeclaration OF AssignmentExpression RIGHT_PAREN Statement
    ;

ExpressionOptional:
    Expression
    | 
    ;
    
LexicalDeclaration:
    LetOrConst 
    // TODO not implemented yet | BindingList
    ;
    
ForDeclaration:
    LetOrConst 
    | ForBinding
    ;
    
ForBinding:
    IDENTIFIER
    /* TODO this is a temp matching with IDENTIFIER, commented out rules match ES6 spec
    BindingIdentifier
    | BindingPattern
    */
    ;

LetOrConst:
    LET
    | CONST
    ;

SwitchStatement:
    SWITCH LEFT_PAREN Expression RIGHT_PAREN CaseBlock
    ;
    
CaseBlock:
    LEFT_BRACE CaseClausesOptional RIGHT_BRACE
    | LEFT_BRACE CaseClausesOptional DefaultClause CaseClausesOptional RIGHT_BRACE
    ;

CaseClauses:
    CaseClause
    | CaseClauses CaseClause
    ;

CaseClausesOptional:
    CaseClauses
    | 
    ;
    
CaseClause:
    CASE Expression COLON StatementListOptional
    ;
    
DefaultClause:
    DEFAULT COLON StatementListOptional
    ;

Expression:
    AssignmentExpression
    | PrimaryExpression
    | EqualityExpression
    ;

PrimaryExpression:
    THIS
    | IdentifierReference
    | Literal
    ;

EqualityExpression:
    Expression EQUAL Expression
    | Expression NOT_EQUAL Expression
    | Expression EXACTLY_EQUAL Expression
    | Expression NOT_EXACTLY_EQUAL Expression
    ;

Literal:
    NullLiteral
    | BooleanLiteral
    | NumericLiteral
    | StringLiteral
    ;

NullLiteral:
    LITERAL_NULL
    ;

BooleanLiteral:
    LITERAL_TRUE
    | LITERAL_FALSE
    ;

NumericLiteral:
    VALUE_INTEGER
    | VALUE_FLOAT
    ;

StringLiteral:
    VALUE_STRING
    ;

AssignmentExpression:
    YieldExpression
    | ArrowFunction
    | LeftHandSideExpression ASSIGNMENT AssignmentExpression
    | LeftHandSideExpression AssignmentOperator AssignmentExpression
    ;

AssignmentOperator:
    MULTIPLICATION_ASSIGNMENT
    | DIVISION_ASSIGNMENT
    | MODULUS_ASSIGNMENT
    | ADDITION_ASSIGNMENT
    | SUBTRACTION_ASSIGNMENT
    | LEFT_SHIFT_ASSIGNMENT
    | SIGNED_RIGHT_SHIFT_ASSIGNMENT
    | UNSIGNED_RIGHT_SHIFT_ASSIGNMENT
    | BITWISE_AND_ASSIGNMENT
    | BITWISE_XOR_ASSIGNMENT
    | BITWISE_OR_ASSIGNMENT
    ;

LeftHandSideExpression:
    CallExpression
    ;

CallExpression:
    SuperCall
    | CallExpression RIGHT_BRACE Expression LEFT_BRACE
    | CallExpression FULL_STOP IdentifierName
    ;

IdentifierReference:
    IDENTIFIER
    ;

IdentifierName:
    IdentifierStart
    | IdentifierName IdentifierPart
    ;

IdentifierStart:
    "$"
    | "_"
    | IDENTIFIER
    ;

IdentifierPart:
    "$"
    | "_"
    | IDENTIFIER
    ;

SuperCall:
    SUPER Arguments
    ;

Arguments:
    LEFT_PAREN RIGHT_PAREN
    ;

YieldExpression:
    YIELD
    | YIELD AssignmentExpression
    ;

ArrowFunction:
    ArrowParameters ARROW_FUNCTION ConciseBody
    ;

ArrowParameters:
    CoverParenthesizedExpressionAndArrowParameterList
    ;

CoverParenthesizedExpressionAndArrowParameterList:
    LEFT_PAREN Expression RIGHT_PAREN
    ;

ConciseBody:
    AssignmentExpression
    | RIGHT_BRACKET FunctionBody LEFT_BRACKET
    ;

FunctionBody:
    FunctionStatementList
    ;

FunctionStatementList:
    StatementList
    ;

%%
void ECMA::ECMA_Parser::error( const location_type &l, const std::string &err_message ) {
    std::cerr << "Error: " << err_message << " at " << l << std::endl;
}