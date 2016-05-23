//
// Created by Freeman on 21/03/2016.
//
#include <stdio.h>
#include "y.tab.h"
#include "ast/ast.hpp"
#include "grammar.tab.h"
#include "lex.yy.h"
#include <stdlib.h>


extern FILE *yyin;
int yyparse(void);
extern ScriptBody *root;
extern int global_var;
extern unsigned int getNewRegister();
/* prototype */
void CodeGeneration(char* inputfile, ScriptBody* root);


int main(int argc, char* argv[])
{
	int global_var=0;

    ESObject* global = new ESObject();
    global->set(new Number(1), new Number(42));
    global->set(new String("y"), Core::plus(global->get(new Number(1)), new Number(4)));
    Console::log(global->get(new String("y")));

    yyin = fopen(argv[1], "r");

    // 'compiled' c file name
    char* inputFile = argv[1];
    char* outputFilename = (char*)malloc(strlen(inputFile) + 3);
    sprintf(outputFilename, "%s.c", inputFile);
    FILE* outputFile = fopen(outputFilename, "w");

    yyparse();
    if (root != NULL) {
        root->dump(0);
        root->genCode(outputFile);
    }
    return 0;
}

char* substring(const char* str, size_t begin, size_t len) 
{ 
  if (str == 0 || strlen(str) == 0 || strlen(str) < begin || strlen(str) < (begin+len)) 
    return 0; 

  return strndup(str + begin, len); 
}