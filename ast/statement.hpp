#pragma once
#include <cstdarg>
#include <cstdio>
#include <cstddef>
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
	bool resolveNames(LexicalScope* scope) {
		if (expr) {
			return expr->resolveNames(scope);
		}
		return false;
	}
};

class StatementList: public Node, public LexicalScope {
private:
  vector<Statement*> *stmts;
public:
  StatementList(vector<Statement*> *stmts):
    stmts(stmts) {};
  void dump(int indent) {
    label(indent, "StatementList\n");
    for (vector<Statement*>::iterator iter = stmts->begin(); iter != stmts->end(); ++iter)
      (*iter)->dump(indent+1);
  }
	bool resolveNames(LexicalScope* scope) {

		bool scoped = true;
		this->parentScope = scope;

		if (stmts) {
			for (std::vector<Statement *>::iterator it = stmts->begin(); it != stmts->end(); ++it) {
				if (*it) {
					Declaration *declaration = dynamic_cast<Declaration *>(*it);
					if (declaration != NULL) {
						symbolTable[declaration->getName()] = declaration;
					}

					if (!(*it)->resolveNames(scope)) {
						scoped = false;
					}
				} else {
					scoped = false;
				}
			}
			return scoped;
		}
		return false;
	}
};

//13.2 Block
class BlockStatement : public Statement {
private:
	StatementList *statementList;

public:
	BlockStatement () {
	}

	BlockStatement (vector<Statement*> *stmts) {
		this->statementList = new StatementList(stmts);
	}

	void dump(int indent) {
		label(indent, "BlockStatement\n");
		indent++;
		if(statementList  != NULL) {
			statementList->dump(indent);
		} else {
			label(indent, "[EMPTY]\n");
		}
	}

	bool resolveNames(LexicalScope* scope) {
		if (statementList) {
			return statementList->resolveNames(scope);
		}
		return false;
	}
};

// 13.13 The try Statement
class TryStatement : public Statement {
private:
	Statement *tryStatement;
	Statement *catchStatement;
	Statement *finallyStatement;

public:
	TryStatement (Statement *tryStatement, Statement *catchStatement, Statement *finallyStatement) {
		this->tryStatement = tryStatement;
		this->catchStatement = catchStatement;
		this->finallyStatement = finallyStatement;
	}

	void dump(int indent) {
		label(indent, "TryStatement\n");
		indent++;
		tryStatement->dump(indent);
		if(catchStatement != NULL) {
			catchStatement->dump(indent);
		}
		if(finallyStatement) {
			finallyStatement->dump(indent);
		}
	}

	bool resolveNames(LexicalScope* scope) {
		bool scoped = true;
		if (tryStatement && !tryStatement->resolveNames(scope)) {
			scoped = false;
		}
		if (catchStatement && !catchStatement->resolveNames(scope)) {
			scoped = false;
		}
		if (finallyStatement && !finallyStatement->resolveNames(scope)) {
			scoped = false;
		}
		return scoped;
	}
};

class CatchStatement : public Statement {
private:
	Expression  *expression;
	Statement *statement;

public:
	CatchStatement(Expression *expression, Statement *statement) {
		this->expression = expression;
		this->statement = statement;
	}

	void dump(int indent) {
		label(indent, "CatchStatement\n");
		indent++;
		expression->dump(indent);
		statement->dump(indent);
	}

	bool resolveNames(LexicalScope* scope) {
		if (expression && statement) {
			return expression->resolveNames(scope) && statement->resolveNames(scope);
		}
		return false;
	}
};

class FinallyStatement : public Statement {
private:
	Statement *statement;
	
public:
	FinallyStatement(Statement *statement) {
		this->statement = statement;
	}

	void dump(int indent) {
		label(indent, "FinallyStatement\n");
		indent++;
		statement->dump(indent);
	}

	bool resolveNames(LexicalScope* scope) {
		if (statement) {
			return statement->resolveNames(scope);
		}
		return false;
	}
};

class ThrowStatement: public Statement{
private:
	Expression* expr;
public:
	ThrowStatement(Expression* expr):
		expr(expr) {};
	void dump(int indent){
		label(indent, "ThrowStatement\n");
		expr->dump(indent+1);
	}

	bool resolveNames(LexicalScope* scope) {
		if (expr) {
			return expr->resolveNames(scope);
		}
		return false;
	}
};

class ReturnStatement: public Statement {
private:
	Expression *expr;

public:
	ReturnStatement() {
		this->expr = NULL;
	}

	ReturnStatement(Expression *expr) {
		this->expr = expr;
	}

	void dump(int indent) {
		label(indent++, "ReturnStatement\n");
		if (this->expr != NULL) {
			expr->dump(indent);
		} else {
			label(indent, "[Empty]\n");
		}
	}

	bool resolveNames(LexicalScope *scope) {
		if (expr) {
			return expr->resolveNames(scope);
		}
		return false;
	};

};

class BreakStatement: public Statement {
private:
	Expression* expr;

public:
	BreakStatement(){
		this->expr = NULL;
	}
	BreakStatement(Expression* expr){
		this->expr = expr;
	}

	void dump(int indent) {
		label(indent++, "BreakStatement\n");
		if(this->expr != NULL){
			expr->dump(indent);
		} else {
			label(indent, "[Empty]\n");
		}
	}

	bool resolveNames(LexicalScope* scope) {
		if (expr) {
			return expr->resolveNames(scope);
		}
		return false;
	}
};

/* 13.6 If Statement
 * http://www.ecma-international.org/ecma-262/6.0/#sec-if-statement
 */
class IfStatement: public Statement {
private:
	Expression *expression;
	Statement *statement;
	Statement *elseStatement;
public:

	// if (expression) { statement }
	IfStatement(Expression *expression, Statement *statement) {
		this->expression = expression;
		this->statement = statement;
	}

	// if (expression) { statement } else { elseStatement }
	IfStatement(Expression *expression, Statement *statement, Statement *elseStatement) {
		this->expression = expression;
		this->statement = statement;
		this->elseStatement = elseStatement;
	}

	void dump(int indent) {
		label(indent, "IfStatement\n");
		indent++;
		expression->dump(indent);
		statement->dump(indent);
		if (elseStatement) {
			elseStatement->dump(indent);
		}
	}

	bool resolveNames(LexicalScope* scope) {
		bool scoped = true;
		if (expression && !expression->resolveNames(scope)) {
			scoped = false;
		}
		if (statement && !statement->resolveNames(scope)) {
			scoped = false;
		}
		if (elseStatement && !elseStatement->resolveNames(scope)) {
			scoped = false;
		}
		return scoped;
	}
};