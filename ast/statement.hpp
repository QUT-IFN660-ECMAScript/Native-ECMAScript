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
private:
static String vinit[];
public:
	virtual unsigned int genCode(FILE* file) = 0;
	virtual unsigned int genStoreCode(FILE* file)=0;
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

	
	unsigned int genCode(FILE* file) {
		expr->genStoreCode(file);
 		return getNewRegister();
	}
	
	unsigned int genStoreCode(FILE* file) {
		return getNewRegister();
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
	
	
	unsigned int genCode(FILE* file) {
		return getNewRegister();
	}
	
	unsigned int genStoreCode(FILE* file) {return getNewRegister();};
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

	
	unsigned int genCode(FILE* file) {
		return getNewRegister();
	}

	unsigned int genStoreCode(FILE* file) {return getNewRegister();};

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

	
	unsigned int genCode(FILE* file) {
		return getNewRegister();
	}

	unsigned int genStoreCode(FILE* file) {return getNewRegister();};

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

	
	
	unsigned int genCode(FILE* file) {
		return getNewRegister();
	}

	unsigned int genStoreCode(FILE* file) {return getNewRegister();};

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

	
	
	unsigned int genCode(FILE* file) {
		return getNewRegister();
	}

	unsigned int genStoreCode(FILE* file) {return getNewRegister();};

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



	unsigned int genCode(FILE* file) {
		return getNewRegister();
	}

	unsigned int genStoreCode(FILE* file) {return getNewRegister();};

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



	unsigned int genCode(FILE* file) {
		return getNewRegister();
	}
	
	unsigned int genStoreCode(FILE* file) {return getNewRegister();};

};

class LabelledItemStatement: public Statement {
private:
	Statement *stmt;
	//FunctionDeclarationStatement *funcStmt;
public:
    LabelledItemStatement() {
        this->stmt = NULL;
    };
    LabelledItemStatement(Statement *stmt) {
        this->stmt = stmt;
    };
    // LabelledItemStatement(Statement *stmt, FunctionDeclarationStatement *funcStmt) {
    //     this->funcStmt = funcStmt;
    // };

	void dump(int indent) {
		label(indent++, "LabelledItemStatement\n");
		if(this->stmt != NULL){
			stmt->dump(indent);
		} else {
			label(indent, "[Empty]\n");
		}
		// if(this->funcStmt != NULL){
		// 	funcStmt->dump(indent);
		// } else {
		// 	label(indent, "[Empty]\n");
		// }
	}
	
	
	
	unsigned int genCode(FILE* file) {
		return getNewRegister();
	}

	unsigned int genStoreCode(FILE* file) {return getNewRegister();};

};


class LabelledStatement: public Statement {
private:
	Expression* expr;
	LabelledItemStatement *stmt;
public:
    LabelledStatement() {
        this->expr = NULL;
    };
    LabelledStatement(Expression *expr) {
        this->expr = expr;
    };
    LabelledStatement(Expression *expr, LabelledItemStatement *stmt) {
        this->expr = expr;
        this->stmt = stmt;
    };

	void dump(int indent) {
		label(indent++, "LabelledStatement\n");
		if(this->expr != NULL){
			expr->dump(indent);
		} else {
			label(indent, "[Empty]\n");
		}
	}



	unsigned int genCode(FILE* file) {
		return getNewRegister();
	}

	unsigned int genStoreCode(FILE* file) {return getNewRegister();};

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



	unsigned int genCode(FILE* file) {
		return getNewRegister();
	}

	unsigned int genStoreCode(FILE* file) {return getNewRegister();};

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

	
	unsigned int genStoreCode(FILE* file) {return getNewRegister();};

	unsigned int genCode(FILE *file) {
		return getNewRegister();
    }
};


class DoWhileIterationStatement : public Statement {
	private:
		Expression *expression;
		Statement *statement;
		
	public:
	DoWhileIterationStatement(Statement *statement,Expression *expression) {
		this->expression = expression;
		this->statement = statement;
	}
	
	void dump(int indent) {
		indent++;
		label(indent, "DoWhileStatement\n");
		expression->dump(indent+ 2);
		statement->dump(indent + 1);
	}

	unsigned int genCode(FILE* file) { return getNewRegister(); }

	unsigned int genStoreCode(FILE* file) {	return getNewRegister(); }
};






