//
// Created by Freeman on 21/03/2016.
//
#include <stdio.h>
#include "ast/node.hpp"

extern FILE *yyin;
extern "C" int yyparse(void);

int main(int argc, char* argv[])
{
    yyin = fopen(argv[1], "r");
    yyparse();
}
