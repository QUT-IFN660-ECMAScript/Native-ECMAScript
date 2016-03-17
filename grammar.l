%{

#include "grammar.tab.h"
void yyerror (char const *s);
static void comment(void);
%}

DIGIT                               [0-9]
CHAR                                [$_a-zA-Z]

%option noyywrap

%%

"//".*				                { /* multi-line comment //one-line one*/ }
break                               { return BREAK; }
case                                { return CASE; }
catch                               { return CATCH; }
class                               { return CLASS; }
const                               { return CONST; }
continue                            { return CONTINUE; }
debugger                            { return DEBUGGER; }
default                             { return DEFAULT; }
delete                              { return DELETE; }
do                                  { return DO; }
else                                { return ELSE; }
export                              { return EXPORT; }
extends                             { return EXTENDS; }
finally                             { return FINALLY; }
for                                 { return FOR; }
function                            { return FUNCTION; }
if                                  { return IF; }
import                              { return IMPORT; }
in                                  { return IN; }
instanceof                          { return INSTANCEOF; }
let                                 { return LET; }
new                                 { return NEW; }
of                                  { return OF; }
return                              { return RETURN; }
super                               { return SUPER; }
switch                              { return SWITCH; }
this                                { return THIS; }
throw                               { return THROW; }
try                                 { return TRY; }
typeof                              { return TYPEOF; }
var                                 { return VAR; }
void                                { return VOID; }
while                               { return WHILE; }
with                                { return WITH; }
yield                               { return YIELD; }

enum                                { return ENUM; }
await                               { return AWAIT; }
implements                          { return IMPLEMENTS; }
interface                           { return INTERFACE; }
package                             { return PACKAGE; }
private                             { return PRIVATE; }
protected                           { return PROTECTED; }
public                              { return PUBLIC; }

Null                                { return LITERAL_NULL; }
true                                { return LITERAL_TRUE; }
false                               { return LITERAL_FALSE; }
undefined                           { return LITERAL_UNDEFINED; }
NaN                                 { return LITERAL_NAN; }

"++"                                { return UNARY_ADD; }
"--"                                { return UNARY_SUBTRACT; }
"!"                                 { return LOGICAL_NOT; }

"*"                                 { return MULTIPLY; }
"/"                                 { return DIVIDE; }
"%"                                 { return MODULO; }

"+"                                 { return ADD; }
"-"                                 { return SUBTRACT; }

"=="                                { return EQUAL; }
"!="                                { return NOT_EQUAL; }
"==="                               { return EXACTLY_EQUAL; }
"!=="                               { return NOT_EXACTLY_EQUAL; }

"<<"                                { return LEFT_SHIFT; }
">>"                                { return SIGNED_RIGHT_SHIFT; }
">>>"                               { return UNSIGNED_RIGHT_SHIFT; }

"<"                                 { return LESS_THAN; }
">"                                 { return GREATER_THAN; }
"<="                                { return LESS_THAN_OR_EQUAL; }
">="                                { return GREATER_THAN_OR_EQUAL; }

"?"                                 { return QUESTION_MARK; }
":"                                 { return COLON; }
"&&"                                { return LOGICAL_AND; }
"||"                                { return LOGICAL_OR; }

"&"                                 { return BITWISE_AND; }
"|"                                 { return BITWISE_OR; }
"^"                                 { return BITWISE_XOR; }
"~"                                 { return BITWISE_NOT; }

"="                                 { return ASSIGNMENT; }
"+="                                { return ADDITION_ASSIGNMENT; }
"-="                                { return SUBTRACTION_ASSIGNMENT; }
"*="                                { return MULTIPLICATION_ASSIGNMENT; }
"/="                                { return DIVISION_ASSIGNMENT; }
"%="                                { return MODULUS_ASSIGNMENT; }
"**="                               { return EXPONENTIATION_ASSIGNMENT; }
"<<="                               { return LEFT_SHIFT_ASSIGNMENT; }
">>="                               { return SIGNED_RIGHT_SHIFT_ASSIGNMENT; }
">>>="                              { return UNSIGNED_RIGHT_SHIFT_ASSIGNMENT; }
"&="                                { return BITWISE_AND_ASSIGNMENT; }
"^="                                { return BITWISE_XOR_ASSIGNMENT; }
"|="                                { return BITWISE_OR_ASSIGNMENT; }
"=>"                                { return ARROW_FUNCTION; }

")"                                 { return RIGHT_PAREN; }
"("                                 { return LEFT_PAREN; }
"}"                                 { return RIGHT_BRACE; }
"{"                                 { return LEFT_BRACE; }
"]"                                 { return RIGHT_BRACKET; }
"["                                 { return LEFT_BRACKET; }
","                                 { return COMMA; }

"..."                               { return ELLIPSIS; }
";"                                 { return SEMICOLON; }

"\""                                { return DOUBLE_QUOTE; }
"'"                                 { return SINGLE_QUOTE; }


{DIGIT}+\.{DIGIT}+	                { yylval.fval = atof(yytext); return VALUE_FLOAT; }
{DIGIT}+                            { yylval.ival = atoi(yytext); return VALUE_INTEGER; }
L?\"(\\.|[^\\"])*\"                 { yylval.sval = strdup(yytext); return VALUE_STRING; }
L?\'(\\.|[^\\"])*\'                 { yylval.sval = strdup(yytext); return VALUE_STRING; }

{CHAR}({DIGIT}|{CHAR})*             { yylval.sval = strdup(yytext); return IDENTIFIER; }
"."                                 { return FULL_STOP; }
"//".*  							/* ignore comments */
[ \t\n]                             /* do nothing */
.                                   { yyerror("Unknown character"); }
%%


static void comment(void) 

{
    int character;
    while ((character = input()) != 0)
    {
	if (character == '*')
        {
            while ((character = input()) == '*');
            if (character == '/')
            {
                return;
            }
            if (character == 0)
            {
                break; 
            }
        }
    }
    yyerror("unterminated comment");
}

extern FILE * yyin;


void yyerror (char const *s) {
    fprintf(stderr, "%s\n", s);
}

