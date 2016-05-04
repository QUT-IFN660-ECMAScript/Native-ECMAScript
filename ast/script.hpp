#pragma once
#include <typeinfo>
#include <cstdio>
#include <iostream>
#include "node.hpp"
#include "statement.hpp"

using namespace std;

class ScriptBody: public Node {
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
        bool scoped = true;
        for (std::vector<Statement*>::iterator it = stmts->begin(); it != stmts->end(); ++it) {
            (*it)->dump(0);
            if (!(*it)->resolveNames(scope)) {
                scoped = false;
            }
        }
        return scoped;
    }
};
