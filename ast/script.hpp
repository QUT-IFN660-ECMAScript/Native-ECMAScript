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

    unsigned int genCode() {
		
		emit("int main() {");
		
		for (std::vector<Statement*>::iterator child = stmts->begin(); child != stmts->end(); ++child) {
			(*child)->genCode();
		}
		emit("\treturn 0;");
		emit("}");
		return getNewRegister();
	}

};
