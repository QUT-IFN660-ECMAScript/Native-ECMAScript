//
// Created by Freeman on 21/03/2016.
//

#include <stdio.h>
#include "y.tab.h"


void yyerror(const char *s) {
    fprintf(stderr, "Parse Error:\n%s\n", s);
}

int main(int argc, char **argv) {
    yyparse();
}