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

  bool resolveNames(LexicalScope* scope) {

     //Add local variable declarations to the symbol_table for this lexical scope
    // for(vector<Statement*>::iterator iter = stmts->begin(); 
    //   iter != stmts->end(); ++iter) {
    //   ExpressionStatement* expressionStatement = dynamic_cast<ExpressionStatement*>(*iter);

    //   if(expressionStatement != NULL) {
    //     Declaration *declaration = dynamic_cast<Declaration*>(expressionStatement->getExpression());
    //     if(declaration != NULL) {
    //       symbolTable[declaration->getName()] = declaration;
    //     }
    //   }
    // }

    for (std::vector<Statement *>::iterator it = stmts->begin(); it != stmts->end(); ++it) {
        if (*it) {
          Declaration *declaration = dynamic_cast<Declaration *>(*it);
          if (declaration != NULL) {
            symbolTable[declaration->getName()] = declaration;
          }
        } 
      }

        bool scoped = true;
        for (std::vector<Statement*>::iterator it = stmts->begin(); it != stmts->end(); ++it) {
            if (*it) {
                if (!(*it)->resolveNames(this)) {
                //     scoped = false;
                }
            }
        }
        return scoped;
    }
};
