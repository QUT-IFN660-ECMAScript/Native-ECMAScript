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

int main(int argc, char* argv[])
{

//    ESValue* x = new Number(42);
//    ESValue* y = Core::plus(x, new Number(4));
//    Console::log(y);

    ESObject* global = new ESObject();
    String* x = new String("x");
    String* y = new String("y");
    global->prototype->set(x, new Number(42));
    Console::log(global->prototype->get(x));
//    global->prototype->set(y, Core::plus(global->prototype->get(x), new Number(4)));

    yyin = fopen(argv[1], "r");
    yyparse();
    if (root != NULL) {
        root->resolveNames(NULL);
        root->dump(0);
    }
    return 0;
}
