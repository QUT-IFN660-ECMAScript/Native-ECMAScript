#pragma once
#include <cstdarg>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <typeinfo>

#include "node.hpp"
#include "expression.hpp"

using namespace std;

class Statement: public Node {

};

class ExpressionStatement: public Statement {
private:
  Expression* expr;
public:
  ExpressionStatement(Expression* expr):
    expr(expr) {};
  void dump(int indent){
    label(indent, "ExpressionStatement\n");
    expr->dump(indent+1);
  }
};
//
// class StatementList: public Node {
// private:
//   vector<Statement*> *stmts;
// public:
//   StatementList(vector<Statement*> *stmts):
//     stmts(stmts) {};
//   void dump(int indent) {
//     label(indent, "StatementList\n");
//     for (vector<Statement*>::iterator iter = stmts->begin(); iter != stmts->end(); ++iter)
//       (*iter)->dump(indent+1);
//   }
// };
