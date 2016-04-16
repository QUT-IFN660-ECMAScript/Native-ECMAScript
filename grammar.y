%{
#include <stdio.h>
#include "y.tab.h"
#include "lex.yy.h"
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
%token TARGET
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
%token LINE_FEED
%token CARRIAGE_RETURN
%token LINE_SEPARATOR
%token PARAGRAPH_SEPARATOR


%union {
    int ival;
    double fval;
    char* sval;
}

%error-verbose

%nonassoc ORDER_ELSE
%nonassoc ELSE

%nonassoc EQUAL
%nonassoc NOT_EQUAL
%nonassoc EXACTLY_EQUAL
%nonassoc NOT_EXACTLY_EQUAL

%%

/*A.5 Scripts and Modules*/

/*15.1 Scripts*/

Script:
    ScriptBody
    ;

ScriptBody:
    StatementList
    ;

/* 12.2 Primary Expression */

PrimaryExpression:
    THIS
    | IdentifierReference
    | Literal
    | ArrayLiteral
    | ObjectLiteral
    | FunctionExpression
    | ClassExpression
    | GeneratorExpression
    /*| RegularExpressionLiteral*/
    /*| TemplateLiteral*/
    | CoverParenthesizedExpressionAndArrowParameterList
    ;

CoverParenthesizedExpressionAndArrowParameterList:
    LEFT_PAREN Expression RIGHT_PAREN
    | LEFT_PAREN RIGHT_PAREN
    | LEFT_PAREN ELLIPSIS BindingIdentifier
    | LEFT_PAREN Expression COMMA ELLIPSIS BindingIdentifier RIGHT_PAREN
    ;

/* FIXME: unused
  ParenthesizedExpression:
    LEFT_PAREN Expression RIGHT_PAREN
    ;*/

/* 12.2.4 Literals */

Literal:
    NullLiteral
    | BooleanLiteral
    | NumericLiteral
    | StringLiteral
    ;


/*12.2.5 Array Initialiser*/

ArrayLiteral:
    LEFT_BRACKET ElisionOptional RIGHT_BRACKET
    | LEFT_BRACKET ElementList RIGHT_BRACKET
    | LEFT_BRACKET ElementList COMMA ElisionOptional RIGHT_BRACKET
    ;

ElementList:
    Elision AssignmentExpression
    | AssignmentExpression
    | Elision SpreadElement
    | SpreadElement
    | ElementList COMMA ElisionOptional AssignmentExpression
    | ElementList COMMA ElisionOptional SpreadElement
    ;

Elision:
    COMMA
    | Elision COMMA
    ;

ElisionOptional:
    Elision
    |
    ;

SpreadElement:
    ELLIPSIS AssignmentExpression
    ;

/*12.2.6 Object Initialiser*/

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

PropertyName:
    LiteralPropertyName
    | ComputedPropertyName
    ;

LiteralPropertyName:
    IdentifierName
    | StringLiteral
    | NumericLiteral
    ;

ComputedPropertyName:
    LEFT_BRACKET AssignmentExpression RIGHT_BRACKET
    ;

CoverInitializedName:
    IdentifierReference Initialiser
    ;

Initialiser:
    ASSIGNMENT AssignmentExpression
    ;

/*12.2.9 Template Literals -- "multiline string" TODO */



/* 12.3 Left Hand Side Expressions */

MemberExpression:
    PrimaryExpression
    | MemberExpression LEFT_BRACKET Expression RIGHT_BRACKET
    | MemberExpression FULL_STOP IdentifierName
    /*MemberExpression TemplateLiteral*/
    | SuperProperty
    | MetaProperty
    | NEW MemberExpression Arguments
    ;

SuperProperty:
    SUPER LEFT_BRACKET Expression RIGHT_BRACKET
    | SUPER FULL_STOP IdentifierName
    ;

MetaProperty:
    NewTarget
    ;

NewTarget:
    NEW FULL_STOP TARGET
    ;

NewExpression:
    MemberExpression
    | NEW NewExpression
    ;

CallExpression:
    MemberExpression Arguments
    | SuperCall
    | CallExpression Arguments
    | CallExpression LEFT_BRACKET Expression RIGHT_BRACKET
    | CallExpression FULL_STOP IdentifierName
    /*| CallExpression TemplateLiteral*/
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
    CallExpression
    | NewExpression
    ;


/*12.4 Postfix Expressions*/

PostfixExpression:
    LeftHandSideExpression
    | LeftHandSideExpression UNARY_ADD
    | LeftHandSideExpression UNARY_SUBTRACT
    ;

/*12.5 Unary Operators*/

UnaryExpression:
    PostfixExpression
    | DELETE UnaryExpression
    | VOID UnaryExpression
    | TYPEOF UnaryExpression
    | UNARY_ADD UnaryExpression
    | UNARY_SUBTRACT UnaryExpression
    | ADD UnaryExpression
    | SUBTRACT UnaryExpression
    | BITWISE_NOT UnaryExpression
    | LOGICAL_NOT UnaryExpression
    ;

/*12.6 Multiplicative Operators*/

MultiplicativeExpression:
    UnaryExpression
    | MultiplicativeExpression MultiplicativeOperator UnaryExpression
    ;

MultiplicativeOperator:
    MULTIPLY
    | DIVIDE
    | MODULO
    ;

/* 12.7 Additive Operators */

AdditiveExpression:
    MultiplicativeExpression
    | AdditiveExpression ADD MultiplicativeExpression
    | AdditiveExpression SUBTRACT MultiplicativeExpression
    ;

/* 12.8 Bitwise Shift Operators */

ShiftExpression:
    AdditiveExpression
    | ShiftExpression LEFT_SHIFT AdditiveExpression
    | ShiftExpression SIGNED_RIGHT_SHIFT AdditiveExpression
    | ShiftExpression UNSIGNED_RIGHT_SHIFT AdditiveExpression
    ;

/* 12.9 Relational Operators */

RelationalExpression:
    ShiftExpression
    | RelationalExpression LESS_THAN ShiftExpression
    | RelationalExpression GREATER_THAN ShiftExpression
    | RelationalExpression LESS_THAN_OR_EQUAL ShiftExpression
    | RelationalExpression GREATER_THAN_OR_EQUAL ShiftExpression
    | RelationalExpression INSTANCEOF ShiftExpression
    | RelationalExpression IN ShiftExpression
    ;

/* 12.10 Equality Operators */

EqualityExpression:
    RelationalExpression
    | EqualityExpression EQUAL RelationalExpression
    | EqualityExpression NOT_EQUAL RelationalExpression
    | EqualityExpression EXACTLY_EQUAL RelationalExpression
    | EqualityExpression NOT_EXACTLY_EQUAL RelationalExpression
    ;

/* 12.11 Binary Bitwise Operators */

BitwiseANDExpression:
    EqualityExpression
    | BitwiseANDExpression BITWISE_AND EqualityExpression
    ;

BitwiseXORExpression:
    BitwiseANDExpression
    | BitwiseXORExpression BITWISE_XOR BitwiseANDExpression
    ;

BitwiseORExpression:
    BitwiseXORExpression
    | BitwiseORExpression BITWISE_OR BitwiseXORExpression
    ;

/* 12.12 Binary Logical Operators */

LogicalANDExpression:
    BitwiseORExpression
    | LogicalANDExpression LOGICAL_AND BitwiseORExpression
    ;

LogicalORExpression:
    LogicalANDExpression
    | LogicalORExpression LOGICAL_OR LogicalANDExpression
    ;

/* 12.13 Conditional Operator ( ? : ) */

ConditionalExpression:
    LogicalORExpression
    | LogicalORExpression QUESTION_MARK AssignmentExpression COLON AssignmentExpression
    ;

/* 12.14 Assignment Operators */
/* 14.6.2.2 Expression Rules */
AssignmentExpression:
    ConditionalExpression
    /*| YieldExpression*/
    | ArrowFunction
    | LeftHandSideExpression ASSIGNMENT AssignmentExpression
    | LeftHandSideExpression AssignmentOperator AssignmentExpression
    | BitwiseANDExpression COLON BitwiseANDExpression BITWISE_AND EqualityExpression
    | BitwiseXORExpression COLON BitwiseXORExpression BITWISE_NOT BitwiseANDExpression
    | BitwiseORExpression COLON BitwiseORExpression BITWISE_OR BitwiseXORExpression
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

/*12.15 Comma Operator ( , )*/

Expression:
    AssignmentExpression
    | Expression COMMA AssignmentExpression
    ;

ExpressionOptional:
    Expression
    |
    ;

/* A.3 Statements */
/* 13 ECMAScript Language: Statements and Declarations */

Statement:
    BlockStatement
    | VariableStatement
    | EmptyStatement
    | ExpressionStatement
    | IfStatement
    | BreakableStatement
    | ContinueStatement
    | BreakStatement
    | ReturnStatement
    | WithStatement
    | LabelledStatement
    | ThrowStatement
    | TryStatement
    | DebuggerStatement
    ;

Declaration:
    HoistableDeclaration
    | ClassDeclaration
    | LexicalDeclaration
    ;

HoistableDeclaration:
    FunctionDeclaration
    | GeneratorDeclaration
    ;

BreakableStatement:
    IterationStatement
    | SwitchStatement
    ;

/*13.2 Block*/

BlockStatement:
    Block
    ;

Block:
    LEFT_BRACE StatementList RIGHT_BRACE
    | LEFT_BRACE RIGHT_BRACE
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

/* 13.3.1 Let and Const Declarations */

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

/* 13.3.2 Variable Statement */

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


/* 13.3.3 Destructuring Binding Patterns */

BindingPattern:
    ObjectBindingPattern
    | ArrayBindingPattern
    ;

ObjectBindingPattern:
    LEFT_BRACE RIGHT_BRACE
    | LEFT_BRACE BindingPropertyList RIGHT_BRACE
    | LEFT_BRACE BindingPropertyList COMMA RIGHT_BRACE
    ;

ArrayBindingPattern:
    LEFT_BRACKET ElisionOptional BindingRestElementOptional RIGHT_BRACKET
    | LEFT_BRACKET BindingElementList RIGHT_BRACKET
    | LEFT_BRACKET BindingElementList COMMA ElisionOptional BindingRestElementOptional  RIGHT_BRACKET
    ;

BindingPropertyList:
    BindingProperty
    | BindingPropertyList COMMA BindingProperty
    ;

BindingElementList:
    BindingElisionElement
    | BindingElementList COMMA BindingElisionElement
    ;

BindingElisionElement:
    ElisionOptional BindingElement
    ;

BindingProperty:
    SingleNameBinding
    | PropertyName COLON BindingElement
    ;

BindingElement:
    SingleNameBinding
    | BindingPattern Initialiser
    | BindingPattern
    ;

SingleNameBinding:
    BindingIdentifier Initialiser
    | BindingIdentifier
    ;

BindingRestElement:
    ELLIPSIS BindingIdentifier
    ;

BindingRestElementOptional:
    BindingRestElement
    |
    ;

/*13.4 Empty Statement*/

EmptyStatement:
    SEMICOLON
    ;

/* 13.5 Expression Statement */

ExpressionStatement:
    Expression SEMICOLON
    ;

/* 13.6 The if Statement */

IfStatement:
    IF LEFT_PAREN Expression RIGHT_PAREN Statement %prec ORDER_ELSE
    | IF LEFT_PAREN Expression RIGHT_PAREN Statement ELSE Statement
    ;

/* 13.7 Iteration Statements */

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
    LetOrConst ForBinding
    ;

ForBinding:
    BindingIdentifier
    | BindingPattern
    ;

/*13.8 The continue Statement*/

ContinueStatement:
    CONTINUE SEMICOLON
    | CONTINUE LabelIdentifier SEMICOLON
    ;

/*13.9 The break Statement*/

BreakStatement:
    BREAK SEMICOLON
    | BREAK LabelIdentifier SEMICOLON
    ;

/*13.10 The return Statement*/

ReturnStatement:
    RETURN SEMICOLON
    | RETURN LabelIdentifier SEMICOLON
    ;

/*13.11 The with Statement*/

WithStatement:
    WITH LEFT_PAREN Expression RIGHT_PAREN Statement
    ;

/*13.12 The switch Statement*/


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

    | CaseClauses
    ;

CaseClause:
    CASE Expression COLON StatementListOptional
    ;

DefaultClause:
    DEFAULT COLON StatementListOptional
    ;

/* 13.13 Labelled Statements */

LabelledStatement:
    LabelIdentifier COLON LabelledItem
    ;

LabelledItem:
    Statement
    | FunctionDeclaration
    ;

/*13.14 The throw Statement*/
ThrowStatement:
    THROW Expression SEMICOLON
    ;

/* 13.15 The try Statement */

TryStatement:
    TRY Block Catch
    | TRY Block Finally
    | TRY Block Catch Finally
    ;

Catch:
    CATCH LEFT_PAREN IDENTIFIER RIGHT_PAREN Block
    ;

Finally:
    FINALLY Block
    ;

/*13.16 The debugger statement*/

DebuggerStatement:
    DEBUGGER SEMICOLON
    ;

/*A.4 Functions and Classes*/
/*14.1 Function Definitions*/
/* Second function declaration anonymous function */
FunctionDeclaration:
    FUNCTION BindingIdentifier LEFT_PAREN FormalParameters RIGHT_PAREN LEFT_BRACE FunctionBody RIGHT_BRACE
    | FUNCTION LEFT_PAREN FormalParameters RIGHT_PAREN LEFT_BRACE FunctionBody RIGHT_BRACE
    ;

FunctionExpression:
    FUNCTION BindingIdentifier LEFT_PAREN FormalParameters RIGHT_PAREN LEFT_BRACE FunctionBody RIGHT_BRACE
    | FUNCTION LEFT_PAREN FormalParameters RIGHT_PAREN LEFT_BRACE FunctionBody RIGHT_BRACE
    ;

StrictFormalParameters:
    FormalParameters
    ;

FormalParameters:

    | FormalParameterList
    ;

FormalParameterList:
    FunctionRestParameter
    | FormalsList
    | FormalsList COMMA FormalParameter
    ;

FormalsList:
    FormalParameter
    | FormalsList COMMA FormalParameter
    ;

FunctionRestParameter:
    BindingRestElement
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

/*14.2 Arrow Function Definitions*/

ArrowFunction:
    ArrowParameters ARROW_FUNCTION ConciseBody
    ;

ArrowParameters:
    BindingIdentifier
    | CoverParenthesizedExpressionAndArrowParameterList
    ;

ConciseBody:
    // TODO missing look-ahead
    AssignmentExpression
    | RIGHT_BRACKET FunctionBody LEFT_BRACKET
    ;

/*
FIXME: unused
ArrowFormalParameters:
    LEFT_PAREN StrictFormalParameters RIGHT_PAREN
    ;*/

/* 14.3 Method Definitions */
MethodDefinition:
    PropertyName LEFT_PAREN StrictFormalParameters RIGHT_PAREN RIGHT_BRACE FunctionBody LEFT_BRACE
    | GeneratorMethod
    | "get" PropertyName LEFT_PAREN RIGHT_PAREN LEFT_BRACE FunctionBody RIGHT_BRACE
    | "set" PropertyName LEFT_PAREN PropertySetParameterList RIGHT_PAREN LEFT_BRACE FunctionBody RIGHT_BRACE
    ;

PropertySetParameterList:
    FormalParameter
    ;

/* 14.4 Generator Function Definitions */

GeneratorMethod:
    MULTIPLY PropertyName LEFT_PAREN StrictFormalParameters RIGHT_PAREN LEFT_BRACE GeneratorBody RIGHT_BRACE
    ;

GeneratorDeclaration:
    FUNCTION MULTIPLY BindingIdentifier LEFT_PAREN FormalParameters RIGHT_PAREN LEFT_BRACE GeneratorBody RIGHT_BRACE
    | FUNCTION MULTIPLY LEFT_PAREN FormalParameters RIGHT_PAREN LEFT_BRACE GeneratorBody RIGHT_BRACE
    ;

GeneratorExpression:
    FUNCTION MULTIPLY BindingIdentifier LEFT_PAREN FormalParameters RIGHT_PAREN LEFT_BRACE GeneratorBody RIGHT_BRACE
    |  FUNCTION MULTIPLY LEFT_PAREN FormalParameters RIGHT_PAREN LEFT_BRACE GeneratorBody RIGHT_BRACE
    ;

GeneratorBody:
    FunctionBody
    ;


/*FIXME: reduce/reduce*/
/*YieldExpression:
    YIELD
    | YIELD AssignmentExpression
    | YIELD MULTIPLY AssignmentExpression
    ;*/

/* 14.5 Class Definitions */

ClassDeclaration:
    CLASS BindingIdentifier ClassTail
    /*| CLASS ClassTail // missing +default*/
    ;

ClassExpression:
     CLASS BindingIdentifierOptional ClassTail
     ;

ClassTail:
    ClassHeritageOptional LEFT_BRACE ClassBodyOptional RIGHT_BRACE
    ;

ClassHeritage:
    EXTENDS LeftHandSideExpression
    ;

ClassHeritageOptional:
    ClassHeritage
    |
    ;

ClassBody:
    ClassElementList
    ;

ClassBodyOptional:
    ClassBody
    |
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

/* A.1 Lexical Grammar */
/* 11.6 Names and Keywords */

IdentifierName:
    IDENTIFIER
    ;
/*
?? shouldnt these be in the lex file?
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
    ;*/

/* 11.8 Literals */
/*
FIXME: unused
DivPunctuator:
    DIVIDE
    | DIVISION_ASSIGNMENT
    ;

RightBracePunctuator:
    RIGHT_BRACE
    ;*/

NullLiteral:
    LITERAL_NULL
    ;

BooleanLiteral:
    LITERAL_TRUE
    | LITERAL_FALSE
    ;

NumericLiteral:
    DecimalLiteral
    /*| BinaryIntegerLiteral
    | OctalIntegerLiteral
    | HexIntegerLiteral*/
    ;

DecimalLiteral:
    DecimalIntegerLiteral
    ;

DecimalIntegerLiteral:
    VALUE_INTEGER
    | VALUE_FLOAT
    ;

StringLiteral:
    VALUE_STRING
    ;


/*12.1 Identifiers*/

IdentifierReference:
    Identifier
    | YIELD
    ;

BindingIdentifier:
    Identifier
    | YIELD
    ;

BindingIdentifierOptional:
    BindingIdentifier
    |
    ;

LabelIdentifier:
    Identifier
    | YIELD
    ;

Identifier:
    IdentifierName
    ;


/*
TODO: modules
ExportDeclaration:
    EXPORT VariableStatement
    ;*/

/* TODO this is required to handle SEMICOLON issues */
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
