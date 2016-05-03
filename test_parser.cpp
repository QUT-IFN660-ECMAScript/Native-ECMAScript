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
extern ScriptBody* root;

int main(int argc, char* argv[]) {
    yyin = fopen(argv[1], "r");
    yyparse();
    if (root != NULL) {
        root->resolveNames(NULL);
        //  root->dump(0);
    }
}
