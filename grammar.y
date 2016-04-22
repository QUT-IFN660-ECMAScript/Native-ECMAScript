%{
#include <cstdio>
#include "y.tab.h"
#include "ast/ast.hpp"
#include "lex.yy.h"


int yylex();

ScriptBody *root;

using namespace std;

%}

%token END_OF_FILE 0
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
%token <ival> VALUE_INTEGER
%token <fval> VALUE_FLOAT
%token <sval> VALUE_STRING
%token <sval> IDENTIFIER
%token LINE_FEED
%token CARRIAGE_RETURN
%token LINE_SEPARATOR
%token PARAGRAPH_SEPARATOR


%union {
    ScriptBody* scriptBody;
    vector<Statement* >* statementList;
    Expression* expression;
    Statement* statement;

    int ival;
    double fval;
    char* sval;
    char* str;
}

%error-verbose

%nonassoc ORDER_ELSE
%nonassoc ELSE

%nonassoc EQUAL
%nonassoc NOT_EQUAL
%nonassoc EXACTLY_EQUAL
%nonassoc NOT_EXACTLY_EQUAL

%type <scriptBody> ScriptBody
%type <statementList> StatementList
%type <statement> Statement StatementListItem ExpressionStatement Block Catch Finally TryStatement
%type <expression> Expression DecimalIntegerLiteral DecimalLiteral NumericLiteral
  Literal PrimaryExpression MemberExpression NewExpression LeftHandSideExpression
  PostfixExpression UnaryExpression MultiplicativeExpression AdditiveExpression
  ShiftExpression RelationalExpression EqualityExpression AssignmentExpression
  ConditionalExpression LogicalANDExpression LogicalORExpression BitwiseORExpression
  BitwiseANDExpression BitwiseXORExpression IdentifierReference BindingIdentifier LabelIdentifier StringLiteral CatchParameter
%type <sval> Identifier IdentifierName

%%

/* 15.1 Scripts
 * http://www.ecma-international.org/ecma-262/6.0/#sec-scripts
 */

Script:
    ScriptBody                                  { root = $1; }
    ;

ScriptBody:
    StatementList                               { $$ = new ScriptBody($1);  }
    ;

/* 15.2.3 Exports
 * http://www.ecma-international.org/ecma-262/6.0/#sec-exports
 */

 /*ExportDeclaration:
    EXPORT VariableStatement
    ;*/

/* 14.5 Class Declarations
 * http://www.ecma-international.org/ecma-262/6.0/#sec-class-declarations
 */


ClassDeclaration:
    CLASS BindingIdentifier ClassTail
    | CLASS ClassTail
    ;
/*
ClassExpression:
     CLASS BindingIdentifier ClassTail
     ;
*/
ClassTail:
    ClassHeritage RIGHT_BRACE ClassBody LEFT_BRACE
    ;

ClassHeritage:
    EXTENDS LeftHandSideExpression
    ;

ClassBody:
    ClassElementList
    ;

ClassElementList:
    ClassElement
    | ClassElementList ClassElement
    ;

ClassElement:
    MethodDefinition
    | "static" MethodDefinition
    | SEMICOLON
    ;

StrictFormalParameters:
    FormalParameters
    ;


/* Required for ArrowFormalParameters
StrictFormalParameters:
    FormalParameters
    ;
*/

/* 14.4 Generator Function Definitions
 * http://www.ecma-international.org/ecma-262/6.0/#sec-generator-function-definitions
 */

YieldExpression:
    YIELD
    | YIELD AssignmentExpression
    ;

/* 14.3 Method Definitions
 * http://www.ecma-international.org/ecma-262/6.0/#sec-method-definitions
 */

MethodDefinition:
    PropertyName LEFT_PAREN StrictFormalParameters RIGHT_PAREN RIGHT_BRACE FunctionBody LEFT_BRACE
 /* | GeneratorMethod */
    | "get" PropertyName LEFT_PAREN RIGHT_PAREN LEFT_BRACE FunctionBody RIGHT_BRACE
    | "set" PropertyName LEFT_PAREN PropertySetParameterList RIGHT_PAREN LEFT_BRACE FunctionBody RIGHT_BRACE
    ;

PropertySetParameterList:
    FormalParameter
    ;

/* 14.2 Arrow Function Definitions
 * http://www.ecma-international.org/ecma-262/6.0/#sec-arrow-function-definitions
 */

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

/* 14.1 Function Definitions
 * http://www.ecma-international.org/ecma-262/6.0/#sec-function-definitions
 */

FunctionDeclaration:
    FUNCTION BindingIdentifier LEFT_PAREN FormalParameters RIGHT_PAREN LEFT_BRACE FunctionBody RIGHT_BRACE
    | FUNCTION LEFT_PAREN FormalParameters RIGHT_PAREN LEFT_BRACE FunctionBody RIGHT_BRACE
    ;

/*
FunctionExpression:
    FUNCTION BindingIdentifier LEFT_PAREN FormalParameters RIGHT_PAREN LEFT_BRACE FunctionBody RIGHT_BRACE
    ;
*/

FormalParameters:
    FormalParameterList
    ;

FormalParameterList:
    /* incomplete */
    FormalsList
    | FormalsList COMMA FormalParameter
    ;

FormalsList:
    FormalParameter
    | FormalsList COMMA FormalParameter
    ;

FormalParameter:
    BindingElement
    ;

FunctionBody:
    FunctionStatementList
    ;

FunctionStatementList:
    StatementList
    ;

/* 13.16 The debugger Statement
 * http://www.ecma-international.org/ecma-262/6.0/#sec-debugger-statement
 */

DebuggerStatement:
    DEBUGGER SEMICOLON
    ;

/* 13.14 The throw Statement
 * http://www.ecma-international.org/ecma-262/6.0/#sec-throw-statement
 */

ThrowStatement:
    THROW Expression SEMICOLON
    ;

/* 13.13 The try Statement
 * http://www.ecma-international.org/ecma-262/6.0/#sec-try-statement
 */

TryStatement:
    TRY Block Catch 	{$$ = new TryStatement($2, $3, NULL);}
    | TRY Block Finally 	{$$ = new TryStatement($2, NULL, $3);}
    | TRY Block Catch Finally 	{$$ = new TryStatement($2, $3, $4);}
    ;

Catch:
    CATCH LEFT_PAREN CatchParameter RIGHT_PAREN Block {$$ = new CatchStatement($3, $5);}
    ;

Finally:
    FINALLY Block 	{$$ = new FinallyStatement($2);}
    ;

CatchParameter:
	BindingIdentifier	{$$ = $1;}
	| BindingPattern

/* 13.13 Labelled Statements
 * http://www.ecma-international.org/ecma-262/6.0/#sec-labelled-statements
 */

LabelledStatement:
    LabelIdentifier COLON LabelledItem
    ;

LabelledItem:
    Statement
    | FunctionDeclaration
    ;

/* 13.12 The switch Statement
 * http://www.ecma-international.org/ecma-262/6.0/#sec-switch-statement
 */

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

/* 13.11 The with Statement
 * http://www.ecma-international.org/ecma-262/6.0/#sec-with-statement
 */

WithStatement:
    WITH LEFT_PAREN Expression RIGHT_PAREN Statement
    ;

/* 13.9 The break Statement
 * http://www.ecma-international.org/ecma-262/6.0/#sec-break-statement
 */

BreakStatement:
    BREAK SEMICOLON
    | BREAK LabelIdentifier SEMICOLON
    ;

/* 13.8 The continue Statement
 * http://www.ecma-international.org/ecma-262/6.0/#sec-continue-statement
 */

ContinueStatement:
    CONTINUE SEMICOLON
    | CONTINUE LabelIdentifier SEMICOLON
    ;

/* 13.7 The return Statement
 * http://www.ecma-international.org/ecma-262/6.0/#sec-return-statement
 */

ReturnStatement:
    RETURN SEMICOLON
    | RETURN Expression SEMICOLON
    ;

/* 13.7 Iteration Statement
 * http://www.ecma-international.org/ecma-262/6.0/#sec-iteration-statements
 */

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

/* 13.6 If Statement
 * http://www.ecma-international.org/ecma-262/6.0/#sec-if-statement
 */

IfStatement:
    IF LEFT_PAREN Expression RIGHT_PAREN Statement %prec ORDER_ELSE
    | IF LEFT_PAREN Expression RIGHT_PAREN Statement ELSE Statement
    ;

 /* 13.5 Expression Statement
  * http://www.ecma-international.org/ecma-262/6.0/#sec-expression-statement
  */

ExpressionStatement:
    Expression SEMICOLON  { $$ = new ExpressionStatement($1); }
    ;

/* 13.4 Empty Statement
 * http://www.ecma-international.org/ecma-262/6.0/#sec-empty-statement
 */

EmptyStatement:
    SEMICOLON
    ;


/* 13.3.3 Destructuting Binding Patterns
 * http://www.ecma-international.org/ecma-262/6.0/#sec-destructuring-binding-patterns
 */

BindingPattern:
    "todo"
    /* to do */
    ;

BindingElement:
    BindingPattern
    | BindingPattern Initialiser
    ;

/* 13.3.2 Variable Statement
 * http://www.ecma-international.org/ecma-262/6.0/#sec-variable-statement
 */

VariableStatement:
    VAR VariableDeclarationList
    ;

VariableDeclarationList:
    VariableDeclaration
    | VariableDeclarationList COMMA VariableDeclaration
    ;

VariableDeclaration:
    BindingIdentifier
    | BindingIdentifier Initialiser
    ;

/* 13.3.1 let and const Declaration
 * http://www.ecma-international.org/ecma-262/6.0/#sec-let-and-const-declarations
 */

LexicalDeclaration:
    LetOrConst BindingList
    // TODO not implemented yet | BindingList
    ;

LetOrConst:
    LET
    | CONST
    ;

BindingList:
    LexicalBinding
    | BindingList COMMA LexicalBinding
    ;

LexicalBinding:
    BindingIdentifier
    | BindingIdentifier Initialiser
    | BindingPattern
    | BindingPattern Initialiser
    ;

/* 13.2 Block
 * http://www.ecma-international.org/ecma-262/6.0/#sec-block
 */

BlockStatement:
    Block
    ;

Block:
    LEFT_BRACE StatementList RIGHT_BRACE 	{$$ = new BlockStatement($2);}
    | LEFT_BRACE RIGHT_BRACE 				{$$ = new BlockStatement();}
    ;

StatementList:
    StatementListItem                   { $$ = new vector<Statement*>; $$->push_back($1); }
    | StatementList StatementListItem   { $$ = $1; $$->push_back($2); }
    ;

StatementListOptional:
    StatementList
    |
    ;

StatementListItem:
    Statement               { $$ = $1; }
    | Declaration
    ;

/* 13 Statements and Declations
 * http://www.ecma-international.org/ecma-262/6.0/#sec-ecmascript-language-statements-and-declarations
 */

Statement:
    BlockStatement
    | VariableStatement
    | EmptyStatement
    | ExpressionStatement   { $$ = $1; }
    | IfStatement
    | BreakableStatement
    | ReturnStatement
    | ContinueStatement
    | BreakStatement
    | WithStatement
    | LabelledStatement
    | ThrowStatement
    | TryStatement
    | DebuggerStatement
    ;

 Declaration:
    /* TODO The below are not implemented yet, see: section 13 of spec for implementation details */
    HoistableDeclaration
    | ClassDeclaration
 /*   | LexicalDeclaration
    */
    /*| ExportDeclaration -- where is this from?*/
    ;

 HoistableDeclaration:
    FunctionDeclaration
/*  | GeneratorDeclaration */
    ;

BreakableStatement:
    IterationStatement
    | SwitchStatement
    ;

/* 12.15 Comma Operator
 * http://www.ecma-international.org/ecma-262/6.0/#sec-comma-operator
 */

Expression:
    AssignmentExpression    {$$ = $1;}
    | PrimaryExpression
    | EqualityExpression
    ;

ExpressionOptional:
    Expression
    |
    ;

/* 12.14 AssignmentOperator
 * http://www.ecma-international.org/ecma-262/6.0/#sec-assignment-operators
 */

AssignmentExpression:
    ConditionalExpression	{$$ = $1;}
    | YieldExpression
    | ArrowFunction
    | LeftHandSideExpression ASSIGNMENT AssignmentExpression {$$ = new AssignmentExpression($1, $3);}
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

/* 12.13 Conditional Operator
 * http://www.ecma-international.org/ecma-262/6.0/#sec-conditional-operator
 */

ConditionalExpression:
    LogicalORExpression	{$$ = $1;}
    | LogicalORExpression QUESTION_MARK AssignmentExpression COLON AssignmentExpression
    ;

/* 12.12 Binary Logic Operators
 * http://www.ecma-international.org/ecma-262/6.0/#sec-binary-logical-operators
 */

LogicalANDExpression:
    BitwiseORExpression	{$$ = $1;}
    | LogicalANDExpression LOGICAL_AND BitwiseORExpression
    ;

LogicalORExpression:
    LogicalANDExpression	{$$ = $1;}
    | LogicalORExpression LOGICAL_OR LogicalANDExpression
    ;

/* 12.11 Binary Bitwise Operators
 * http://www.ecma-international.org/ecma-262/6.0/#sec-binary-bitwise-operators
 */

BitwiseANDExpression:
    EqualityExpression {$$ = $1;}
    | BitwiseANDExpression BITWISE_AND EqualityExpression
    ;

BitwiseXORExpression:
    BitwiseANDExpression {$$ = $1;}
    | BitwiseXORExpression BITWISE_XOR BitwiseANDExpression
    ;

BitwiseORExpression:
    BitwiseXORExpression	{$$ = $1;}
    | BitwiseORExpression BITWISE_OR BitwiseXORExpression
    ;

/* 12.10 Equality Operators
 * http://www.ecma-international.org/ecma-262/6.0/#sec-equality-operators
 */

EqualityExpression:
    RelationalExpression	{$$ = $1;}
    ;

/* 12.9 Relational Operators
 * http://www.ecma-international.org/ecma-262/6.0/#sec-relational-operators
 */

RelationalExpression:
    ShiftExpression	{$$ = $1;}
    /*
    | Expression EQUAL Expression
    | Expression NOT_EQUAL Expression
    | Expression EXACTLY_EQUAL Expression
    | Expression NOT_EXACTLY_EQUAL Expression
    */
    ;

/* 12.8 Bitwise Shift Operators
 * http://www.ecma-international.org/ecma-262/6.0/#sec-bitwise-shift-operators
 */

ShiftExpression:
    AdditiveExpression	{$$ = $1;}
	| ShiftExpression LEFT_SHIFT AdditiveExpression
	| ShiftExpression SIGNED_RIGHT_SHIFT AdditiveExpression
	| ShiftExpression UNSIGNED_RIGHT_SHIFT  
    ; 

/* 12.7 Additive Operators
 * http://www.ecma-international.org/ecma-262/6.0/#sec-additive-operators
 */

AdditiveExpression:
    MultiplicativeExpression	{$$ = $1;}
    ;

/* 12.6 Multiplicative Operators
 * http://www.ecma-international.org/ecma-262/6.0/#sec-multiplicative-operators
 */

MultiplicativeExpression:
    UnaryExpression	{ $$ = $1; }
    ;

/* 12.5 Unary Operators
 * http://www.ecma-international.org/ecma-262/6.0/#sec-unary-operators
 */

UnaryExpression:
    PostfixExpression	{ $$ = $1; }
	| DELETE UnaryExpression
	| VOID UnaryExpression
	| TYPEOF UnaryExpression
	| UNARY_ADD UnaryExpression
	| UNARY_SUBTRACT UnaryExpression
	/* | ADD UnaryExpression 		
	| SUBTRACT UnaryExpression */
	| BITWISE_NOT UnaryExpression
	| LOGICAL_NOT UnaryExpression
    ;

/* 12.4 Postfix Expression
 * http://www.ecma-international.org/ecma-262/6.0/#sec-postifx-expression
 */

PostfixExpression:
    LeftHandSideExpression	{ $$ = $1; }
    ;

/* 12.3 Left-Hand-Side Expressions
 * http://www.ecma-international.org/ecma-262/6.0/#sec-left-hand-side-expressions
 */

MemberExpression:
    PrimaryExpression	{ $$ = $1; }
    ;

NewExpression:
    MemberExpression	{ $$ = $1; }
    ;

CallExpression:
    SuperCall
    | CallExpression RIGHT_BRACE Expression LEFT_BRACE
    | CallExpression FULL_STOP IdentifierName
    ;

SuperCall:
    SUPER Arguments
    ;

Arguments:
    LEFT_PAREN RIGHT_PAREN
    | LEFT_PAREN ArgumentList RIGHT_PAREN
    ;

ArgumentList:
    AssignmentExpression
    | ArgumentList COMMA AssignmentExpression
    ;

LeftHandSideExpression:
    NewExpression	{ $$ = $1; }
    | CallExpression
    ;

/* 12.2.6 Object Initialiser
 * http://www.ecma-international.org/ecma-262/6.0/#sec-object-initialiser
 */

PropertyName:
    LiteralPropertyName
    | ComputedPropertyName
    ;

LiteralPropertyName:
    IdentifierName
    | StringLiteral
    | NumericLiteral
    ;

Initialiser:
  ASSIGNMENT AssignmentExpression
  ;

ObjectLiteral:
	LEFT_BRACE RIGHT_BRACE
	| LEFT_BRACE PropertyDefinitionList RIGHT_BRACE
	| LEFT_BRACE PropertyDefinitionList COMMA RIGHT_BRACE
	;

PropertyDefinitionList:
	PropertyDefinition
	| PropertyDefinitionList COMMA PropertyDefinition
	;

PropertyDefinition:
	IdentifierReference
	| CoverInitializedName
	| PropertyName COLON AssignmentExpression
	| MethodDefinition
	;

ComputedPropertyName:
	LEFT_BRACKET AssignmentExpression RIGHT_BRACKET
	;

CoverInitializedName:
	IdentifierReference Initialiser
	;

/* 12.2.5 Array Initialiser
 * http://www.ecma-international.org/ecma-262/6.0/#sec-array-initializer
 */

ElementList:
    Elision AssignmentExpression
    | AssignmentExpression
    | Elision SpreadElement
    | SpreadElement
    | ElementList COMMA Elision AssignmentExpression
    | ElementList COMMA AssignmentExpression
    | ElementList COMMA Elision SpreadElement
    | ElementList COMMA SpreadElement
    ;

Elision:
    COMMA
    | Elision COMMA
    ;

SpreadElement:
    ELLIPSIS AssignmentExpression
    ;

/* 12.2.4 Literals
 * http://www.ecma-international.org/ecma-262/6.0/#sec-primary-expression-literals
 */

Literal:
    NullLiteral
    | BooleanLiteral
    | NumericLiteral	{$$=$1;}
    | StringLiteral		{$$=$1;}
    ;

ArrayLiteral:
    LEFT_BRACKET RIGHT_BRACKET
    | LEFT_BRACKET Elision RIGHT_BRACKET
    | LEFT_BRACKET ElementList RIGHT_BRACKET
    | LEFT_BRACKET ElementList COMMA Elision RIGHT_BRACKET
    | LEFT_BRACKET ElementList COMMA RIGHT_BRACKET
    ;

NullLiteral:
    LITERAL_NULL
    ;

BooleanLiteral:
    LITERAL_TRUE
    | LITERAL_FALSE
    ;

StringLiteral:
    VALUE_STRING			 { $$ = new StringLiteralExpression($1); }
    ;

/* 12.2 Primary Expression
 * http://www.ecma-international.org/ecma-262/6.0/#sec-primary-expression
 */

PrimaryExpression:
    THIS
    | IdentifierReference { $$ = $1; }
    | Literal	{ $$ = $1; }
    | ArrayLiteral
    | ObjectLiteral
    ;

/* 12.1 Identifier
 * http://www.ecma-international.org/ecma-262/6.0/#sec-identifiers
 */

IdentifierReference:
    Identifier                              { $$ = new IdentifierExpression($1); }
    ;

BindingIdentifier:
    Identifier                              { $$ = new IdentifierExpression($1); }
    | YIELD
    ;

LabelIdentifier:
    Identifier                              { $$ = new IdentifierExpression($1); }
    | YIELD
    ;

Identifier:
    IdentifierName                          { $$ = $1; }
    ;

/* 11.8.3 Numeric Literals
 * http://www.ecma-international.org/ecma-262/6.0/#sec-literals-numeric-literals
 */

NumericLiteral:
    DecimalLiteral	                         { $$ = $1; }
    ;

DecimalLiteral:
    DecimalIntegerLiteral	                   { $$ = $1; }
    ;

DecimalIntegerLiteral:
    VALUE_INTEGER	                           { $$ = new IntegerLiteralExpression($1); }
    ;

/* 11.6 Name and Keywords
 * http://www.ecma-international.org/ecma-262/6.0/#sec-names-and-keywords
 */
 IdentifierName:
     /*IdentifierStart
     | IdentifierName IdentifierPart*/
     IDENTIFIER                              { $$ = $1; }
     ;

/*


IdentifierStart:
    "$"
    | "_"
    | IDENTIFIER
    ;

IdentifierPart:
    "$"
    | "_"
    | IDENTIFIER
    ;*/
/* 11.3 Line Terminators
 * http://www.ecma-international.org/ecma-262/6.0/#sec-line-terminators
 */

/*LineTerminator:
    LINE_FEED
    | CARRIAGE_RETURN
    | LINE_SEPARATOR
    | PARAGRAPH_SEPARATOR
    ;

LineTerminatorSequence:
    LINE_FEED
    | CARRIAGE_RETURN
    | LINE_SEPARATOR
    | PARAGRAPH_SEPARATOR
    | CARRIAGE_RETURN LINE_FEED
    ;*/
%%
