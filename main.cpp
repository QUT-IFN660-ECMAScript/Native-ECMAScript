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

int Node::registerIndex = 0;

int main(int argc, char* argv[])
{
   // ESValue* x = new Number(42);
   // ESValue* y = Core::plus(x, new Number(4));
   // Console::log(y);

    // ESObject* global = new ESObject();
    // global->set(new Number(1), new Number(42));
    // global->set(new String("y"), Core::plus(global->get(new Number(1)), new Number(4)));
    // Console::log(global->get(new String("y")));

    yyin = fopen(argv[1], "r");
    yyparse();
    if (root != NULL) {
        root->resolveNames(NULL);
        root->dump(0);


        /*
        So far, can only generates DecimalIntegerLiteralExpression, e.x. 5 + 6/2 + 5*10
        */
        FILE* outputFile = fopen("../CodeGeneration.ouput", "w");//output file is in the same folder with Native-ECMAScript
        root->genCode(outputFile);
    }
    return 0;
}
