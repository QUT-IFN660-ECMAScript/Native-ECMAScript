%{
#include <string>
#include "ECMA_Scanner.hpp"

using namespace std;
#undef  YY_DECL
#define YY_DECL int ECMA::ECMA_Scanner::yylex(ECMA::ECMA_Parser::semantic_type * const lval, ECMA::ECMA_Parser::location_type *location)

typedef ECMA::ECMA_Parser::token token;

#define YY_NO_UNISTD_H



std::string* stringbuffer = new std::string;

%}
%option debug
%option nodefault
%option yyclass="ECMA::ECMA_Scanner"
%option noyywrap
%option c++

DIGIT                               [0-9]
CHAR                                [$_a-zA-Z]

%x MULTILINE_STRING

%%
%{          // Code executed at the beginning of yylex
            yylval = lval;
%}
[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/]       { /* DO NOTHING */ }
"//".*                              { /* multi-line comment //one-line one*/ }

break                               { return token::BREAK; }
case                                { return token::CASE; }
catch                               { return token::CATCH; }
class                               { return token::CLASS; }
const                               { return token::CONST; }
continue                            { return token::CONTINUE; }
debugger                            { return token::DEBUGGER; }
default                             { return token::DEFAULT; }
delete                              { return token::DELETE; }
do                                  { return token::DO; }
else                                { return token::ELSE; }
export                              { return token::EXPORT; }
extends                             { return token::EXTENDS; }
finally                             { return token::FINALLY; }
for                                 { return token::FOR; }
function                            { return token::FUNCTION; }
if                                  { return token::IF; }
import                              { return token::IMPORT; }
in                                  { return token::IN; }
instanceof                          { return token::INSTANCEOF; }
let                                 { return token::LET; }
new                                 { return token::NEW; }
of                                  { return token::OF; }
return                              { return token::RETURN; }
super                               { return token::SUPER; }
switch                              { return token::SWITCH; }
this                                { return token::THIS; }
throw                               { return token::THROW; }
try                                 { return token::TRY; }
typeof                              { return token::TYPEOF; }
var                                 { return token::VAR; }
void                                { return token::VOID; }
while                               { return token::WHILE; }
with                                { return token::WITH; }
yield                               { return token::YIELD; }

enum                                { return token::ENUM; }
await                               { return token::AWAIT; }
implements                          { return token::IMPLEMENTS; }
interface                           { return token::INTERFACE; }
package                             { return token::PACKAGE; }
private                             { return token::PRIVATE; }
protected                           { return token::PROTECTED; }
public                              { return token::PUBLIC; }

null                                { return token::LITERAL_NULL; }
true                                { return token::LITERAL_TRUE; }
false                               { return token::LITERAL_FALSE; }
undefined                           { return token::LITERAL_UNDEFINED; }
NaN                                 { return token::LITERAL_NAN; }

"++"                                { return token::UNARY_ADD; }
"--"                                { return token::UNARY_SUBTRACT; }
"!"                                 { return token::LOGICAL_NOT; }

"*"                                 { return token::MULTIPLY; }
"/"                                 { return token::DIVIDE; }
"%"                                 { return token::MODULO; }

"+"                                 { return token::ADD; }
"-"                                 { return token::SUBTRACT; }

"=="                                { return token::EQUAL; }
"!="                                { return token::NOT_EQUAL; }
"==="                               { return token::EXACTLY_EQUAL; }
"!=="                               { return token::NOT_EXACTLY_EQUAL; }

"<<"                                { return token::LEFT_SHIFT; }
">>"                                { return token::SIGNED_RIGHT_SHIFT; }
">>>"                               { return token::UNSIGNED_RIGHT_SHIFT; }

"<"                                 { return token::LESS_THAN; }
">"                                 { return token::GREATER_THAN; }
"<="                                { return token::LESS_THAN_OR_EQUAL; }
">="                                { return token::GREATER_THAN_OR_EQUAL; }

"?"                                 { return token::QUESTION_MARK; }
":"                                 { return token::COLON; }
"&&"                                { return token::LOGICAL_AND; }
"||"                                { return token::LOGICAL_OR; }

"&"                                 { return token::BITWISE_AND; }
"|"                                 { return token::BITWISE_OR; }
"^"                                 { return token::BITWISE_XOR; }
"~"                                 { return token::BITWISE_NOT; }

"="                                 { return token::ASSIGNMENT; }
"+="                                { return token::ADDITION_ASSIGNMENT; }
"-="                                { return token::SUBTRACTION_ASSIGNMENT; }
"*="                                { return token::MULTIPLICATION_ASSIGNMENT; }
"/="                                { return token::DIVISION_ASSIGNMENT; }
"%="                                { return token::MODULUS_ASSIGNMENT; }
"**="                               { return token::EXPONENTIATION_ASSIGNMENT; }
"<<="                               { return token::LEFT_SHIFT_ASSIGNMENT; }
">>="                               { return token::SIGNED_RIGHT_SHIFT_ASSIGNMENT; }
">>>="                              { return token::UNSIGNED_RIGHT_SHIFT_ASSIGNMENT; }
"&="                                { return token::BITWISE_AND_ASSIGNMENT; }
"^="                                { return token::BITWISE_XOR_ASSIGNMENT; }
"|="                                { return token::BITWISE_OR_ASSIGNMENT; }
"=>"                                { return token::ARROW_FUNCTION; }

")"                                 { return token::RIGHT_PAREN; }
"("                                 { return token::LEFT_PAREN; }
"}"                                 { return token::RIGHT_BRACE; }
"{"                                 { return token::LEFT_BRACE; }
"]"                                 { return token::RIGHT_BRACKET; }
"["                                 { return token::LEFT_BRACKET; }
","                                 { return token::COMMA; }
"."                                 { return token::FULL_STOP; }
"..."                               { return token::ELLIPSIS; }
";"                                 { return token::SEMICOLON; }

"\""                                { return token::DOUBLE_QUOTE; }
"'"                                 { return token::SINGLE_QUOTE; }

{DIGIT}+\.{DIGIT}+                  { yylval->fval = atof(yytext); return token::VALUE_FLOAT; }
{DIGIT}+                            { yylval->ival = atoi(yytext); return token::VALUE_INTEGER; }
L?\"(\\.|[^\\"])*\"                 { yylval->sval = new std::string(yytext); return token::VALUE_STRING; }
L?\'(\\.|[^\\"])*\'                 { yylval->sval = new std::string(yytext); return token::VALUE_STRING; }
\`                                  {
                                      BEGIN(MULTILINE_STRING);
                                      stringbuffer = new std::string("");
                                    }
<MULTILINE_STRING>\`                {
                                      BEGIN(INITIAL);
                                      //TODO yylval->build<std::string>(stringbuffer);
                                      return token::VALUE_STRING;
                                    }
<MULTILINE_STRING>\n                ;
<MULTILINE_STRING>.                 {
                                      //TODO stringbuffer += new std::string(yytext);
                                    }

{CHAR}({DIGIT}|{CHAR})*             { yylval->sval = new std::string(yytext); return token::IDENTIFIER; }

[ \t\n]                             { loc->lines(); }
<<EOF>>                             { return token::END_OF_FILE; }
.                                   { cout << "Unknown character" << endl; }
%%
