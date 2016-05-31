//
// Created by Freeman on 21/03/2016.
//
#include <stdio.h>
#include "y.tab.h"
#include "ast/ast.hpp"
#include "grammar.tab.h"
#include "lex.yy.h"
#include <stdlib.h>
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <cstdarg>
#include <cstdio>
#include <string>

extern FILE *yyin;
int yyparse(void);
extern ScriptBody *root;
extern int global_var;
ESObject* globalObj;

extern std::map<int, vector<std::string> > codeScope; // this really should be named something better...?
extern int codeScopeDepth;
extern std::vector<std::string> functionDefinitions;


extern unsigned int getNewRegister();

// int Node::registerIndex = 0;

int main(int argc, char* argv[]) {
	int global_var=0;

    globalObj = new ESObject();
    codeScopeDepth = 0;

    yyin = fopen(argv[1], "r");

    // 'compiled' c file name
    char* inputFile = argv[1];
    char* outputFilename = (char*)malloc(strlen(inputFile) + 3);
    sprintf(outputFilename, "%s.c", inputFile);
    FILE* outputFile = fopen(outputFilename, "w");

    yyparse();

    fprintf(outputFile, "#include \"./runtime/core.hpp\"\n");
    fprintf(outputFile, "#include \"./runtime/console.hpp\"\n");
    fprintf(outputFile, "#include \"./scope/reference.hpp\"\n");
    fprintf(outputFile, "\n");
	fprintf(outputFile, "ESObject* globalObj = new ESObject();\n\n");
    if (root != NULL) {
        root->dump(0);
        root->genCode();

//        printf("printing scoped IR:\n");
        for (std::vector<std::string>::iterator iter = functionDefinitions.begin(); iter != functionDefinitions.end(); ++iter) {
            std::string s = (*iter);
            const char* c = s.c_str();
            fprintf(outputFile, "%s\n", c);
        }

        for (std::vector<std::string>::iterator iter = codeScope[codeScopeDepth].begin(); iter != codeScope[codeScopeDepth].end(); ++iter) {
            std::string s = (*iter);
            const char* c = s.c_str();
            fprintf(outputFile, "%s\n", c);
        }
        fprintf(outputFile, "\n");

    }
    return 0;
}

char* substring(const char* str, size_t begin, size_t len) { 
  if (str == 0 || strlen(str) == 0 || strlen(str) < begin || strlen(str) < (begin+len)) 
    return 0; 

  return strndup(str + begin, len); 
}