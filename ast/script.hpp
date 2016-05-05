#pragma once
#include <typeinfo>
#include <cstdio>
#include <iostream>
#include "node.hpp"
#include "statement.hpp"
#include "declaration.hpp"

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

  bool resolveNames(LexicalScope* scope) {
    this->parentScope = scope;
    bool allOk = true;

    //Add local variable declarations to the symbol_table for this lexical scope
    for(vector<Statement*>::iterator iter = stmts->begin(); 
      iter != stmts->end(); ++iter) {
      ExpressionStatement* expressionStatement = dynamic_cast<ExpressionStatement*>(*iter);

      if(expressionStatement != NULL) {
        Declaration *declaration = dynamic_cast<Declaration*>(expressionStatement->getExpression());
        if(declaration != NULL) {
          symbol_table[declaration->getName()] = declaration;
        }
      }
    }

    for(vector<Statement*>::iterator iter = stmts->begin(); 
      iter != stmts->end(); ++iter) {
      bool b = (*iter)->resolveNames(this);
      if (b == false) {
        allOk = false;
      }
    }

    return allOk;
  }
};
