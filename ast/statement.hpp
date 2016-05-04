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
};

class StatementList: public Node {
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
};

class ReturnStatement: public Statement {
private:
	Expression* expr;

public:
	ReturnStatement(){
		this->expr = NULL;
	}
	ReturnStatement(Expression* expr){
		this->expr = expr;
	}

	void dump(int indent) {
		label(indent++, "ReturnStatement\n");
		if(this->expr != NULL){
			expr->dump(indent);
		} else {
			label(indent, "[Empty]\n");
		}
	}
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
};


class IterationStatement : public Statement {
	private:
		Expression *expression;
		Statement *statement;
		
	public:
	/* while expression statement */
	IterationStatement(Expression *expression, Statement *statement) {
		this->expression = expression;
		this->statement = statement;
	}
	
	void dump(int indent) {
		indent++;
		label(indent, "WhileStatement\n");
		expression->dump(indent+ 1);
		statement->dump(indent + 2);
	}
};


class DoWhileIterationStatement : public Statement {
	private:
		Expression *expression;
		Statement *statement;
		
	public:
	/* while expression statement */
	DoWhileIterationStatement(Statement *statement,Expression *expression) {
		this->expression = expression;
		this->statement = statement;
	}
	
	void dump(int indent) {
		indent++;
		label(indent, "DoWhileStatement\n");
		statement->dump(indent + 1);
		expression->dump(indent+ 2);
		
	}
};



















