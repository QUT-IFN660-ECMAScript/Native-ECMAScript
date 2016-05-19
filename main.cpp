//
// Created by Freeman on 21/03/2016.
//
#include <stdio.h>
#include "y.tab.h"
#include "ast/ast.hpp"
#include "grammar.tab.h"
#include "lex.yy.h"


extern FILE *yyin;
int yyparse(void);
extern ScriptBody *root;
extern int global_var;

/* prototype */
void CodeGeneration(char* inputfile, ScriptBody* root);

int main(int argc, char* argv[])
{
	int global_var=0;
//    ESValue* x = new Number(42);
//    ESValue* y = Core::plus(x, new Number(4));
//    Console::log(y);

    ESObject* global = new ESObject();
    global->set(new Number(1), new Number(42));
    global->set(new String("y"), Core::plus(global->get(new Number(1)), new Number(4)));
    Console::log(global->get(new String("y")));

    yyin = fopen(argv[1], "r");
    yyparse();
    if (root != NULL) {
   //     root->resolveNames(NULL);
        CodeGeneration(argv[1], root);
        root->dump(0);
    }
    return 0;
}



void CodeGeneration(char* inputfile, ScriptBody* root) {
	char* outputFilename = (char*)malloc(strlen(inputfile) + 3);
	sprintf(outputFilename, "%s.il", inputfile);
	FILE* outputFile = fopen(outputFilename, "w");

	
/*	root->emit(outputFile, ".assembly %s {}", inputfile);
	root->emit(outputFile, ".class %s {", inputfile);
	root->emit(outputFile, ".method static void Main(string[] args) {");
	root->emit(outputFile, ".entrypoint"); */

	root->GenCode(outputFile);
	
/*	root->emit(outputFile, "ret");
	root->emit(outputFile, "}"); // end of Main
	root->emit(outputFile, "}"); // end of class */
}
