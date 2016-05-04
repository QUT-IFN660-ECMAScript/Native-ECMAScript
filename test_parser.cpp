//
// Created by Freeman on 21/03/2016.
//
#include <stdio.h>
#include <unistd.h>
#include "y.tab.h"
#include "ast/ast.hpp"
#include "grammar.tab.h"
#include "lex.yy.h"
#include <string>
#include <iostream>

using namespace std;

extern FILE *yyin;
int yyparse(void);
extern ScriptBody* root;

int main(int argc, char* argv[])
{
  if (argc == 3) {
    string arg = argv[1];
    if (arg == "-d") {
      char* filename = argv[2];
      if (access(filename, F_OK) != -1) {
        yyin = fopen(filename, "r");
        yyparse();
        if(root != NULL) {
          root->resolveNames(NULL);
          root->dump(0);
        }
        fclose(yyin);
        return 0;
      }
    }
  } else if (argc == 2) {
    char* filename = argv[1];
    if (access(filename, F_OK) != -1) {
      yyin = fopen(filename, "r");
      yyparse();
      fclose(yyin);
      return 0;
    }
  }

  cerr << "Usage: test_parser [-d] script.js\n";
  return 1;
}
