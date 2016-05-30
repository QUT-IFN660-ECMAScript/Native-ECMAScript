#pragma once
#include <cstdarg>
#include <cstdio>
#include <cstddef>
#include <string>
#include <vector>
#include <iostream>
#include <typeinfo>
#include "statement.hpp"
#include "expression.hpp"
#include "node.hpp"

using namespace std;

extern int global_var;

class Declaration: public Node {
public:
	virtual unsigned int genCode(FILE* file) = 0;
	virtual unsigned int genStoreCode(FILE* file)=0;
};



class FunctionDeclaration : public Declaration, public Expression, public Statement {

private:
	Expression* expression;
	vector<Expression*> *params;
	vector<Statement*> *statements;
	
public:

	
	
	FunctionDeclaration(Expression* expression, vector<Expression*> *params, vector<Statement*>  *statements) {
		this->expression = expression;
		this->params = params;
		this->statements = statements;
	}
	
	void dump(int indent) {
		Declaration::label(indent, "FunctionDeclaration\n");
     	expression->dump(++indent);
       	for (vector<Expression*>::iterator iter = params->begin(); iter != params->end(); ++iter) {
          (*iter)->dump(++indent);
		}
     	for (vector<Statement*>::iterator iter = statements->begin(); iter != statements->end(); ++iter)
          (*iter)->dump(++indent);
     	
     }
     
    
    
    unsigned int genCode(FILE* file) 	{
        return getNewRegister();
    }
	
	
	unsigned int genStoreCode(FILE* file) {
		
	}
};	