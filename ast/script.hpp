#pragma once
#include <typeinfo>
#include <cstdio>
#include <iostream>
#include "node.hpp"
#include "statement.hpp"

using namespace std;

class ScriptBody: public Node, public LexicalScope {
private:
  vector<Statement*> *stmts;
public:  

	
  ScriptBody(vector<Statement*> *stmts):
    stmts(stmts) {};
  void dump(int indent) {
    label(indent, "ScriptBody\n");
    vector<Statement*>::iterator iter;
    for (iter = stmts->begin(); iter != stmts->end(); ++iter) {
      (*iter)->dump(indent+1);
    }
  }
  
  void emit(FILE* outputfile, char* fmt, ...) {	
  		printf("ASDDASSA");
		va_list args;
		va_start(args, fmt);
		vfprintf(outputfile, fmt, args);
		fprintf(outputfile, "\n");
		va_end(args);
	}

 
    
    void GenCode(FILE* file) 	{
		for (std::vector<Statement*>::iterator child = stmts->begin(); child != stmts->end(); ++child) {
			(*child)->GenCode(file);
		}
	}
	
	void GenStoreCode(FILE* file) {}
   
};
