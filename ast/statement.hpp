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


extern std::map<int, std::vector<std::string> > codeScope;
extern std::vector<std::string> functionDefinitions;

using namespace std;


class Statement: public Node {
private:
static String vinit[];
public:
	virtual unsigned int genCode() = 0;
	virtual unsigned int genStoreCode()=0;
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


	unsigned int genCode() {
		expr->genStoreCode();
 		return getNewRegister();
	}

	unsigned int genStoreCode() {
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


	unsigned int genCode() {
		return getNewRegister();
	}

	unsigned int genStoreCode() {return getNewRegister();};

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


	unsigned int genCode() {
		return getNewRegister();
	}

	unsigned int genStoreCode() {return getNewRegister();};

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


	unsigned int genCode() {
		return getNewRegister();
	}

	unsigned int genStoreCode() {return getNewRegister();};

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



	unsigned int genCode() {
		return getNewRegister();
	}

	unsigned int genStoreCode() {return getNewRegister();};

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



	unsigned int genCode() {
		return getNewRegister();
	}

	unsigned int genStoreCode() {return getNewRegister();};

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



	unsigned int genCode() {
		return getNewRegister();
	}

	unsigned int genStoreCode() {return getNewRegister();};

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

	unsigned int genCode() {
		if (this->expr != NULL) {
			this->expr->genStoreCode();
			unsigned int reg = getNewRegister();
			emit("\treturn r%d;", reg - 1);
			return reg;
		}
		else{
			emit("\treturn;");
			return getNewRegister();
		}
	}

	unsigned int genStoreCode() {return getNewRegister();};

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



	unsigned int genCode() {
		return getNewRegister();
	}

	unsigned int genStoreCode() {return getNewRegister();};

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



	unsigned int genCode() {
		return getNewRegister();
	}

	unsigned int genStoreCode() {return getNewRegister();};

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



	unsigned int genCode() {
		return getNewRegister();
	}

	unsigned int genStoreCode() {return getNewRegister();};

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


	unsigned int genStoreCode() {
		return getNewRegister();
	};

	unsigned int genCode() {
		unsigned int registerNumber = getNewRegister();
		emit("\t// r%d", registerNumber);
		unsigned int expressionRegister = expression->genCode();
		emit("\t// r%d evaluate some boolean expression if true skip to register after all statements", expressionRegister);
		unsigned int statementRegister = statement->genCode();
		emit("\t// r%d jmp r%d", statementRegister, registerNumber);
		registerNumber = getNewRegister();
		emit("\t// r%d skip to here if expression is true", registerNumber);

		return registerNumber;
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

	unsigned int genCode() { return getNewRegister(); }

	unsigned int genStoreCode() {	return getNewRegister(); }
};


class WithStatement : public Statement {
	private:
		Expression *expression;
		Statement *statement;

	public:
	/* with expression statement */
	WithStatement(Expression *expression, Statement *statement) {
		this->expression = expression;
		this->statement = statement;
	}

	void dump(int indent) {
		indent++;
		label(indent, "WithStatement\n");
		expression->dump(indent+ 1);
		statement->dump(indent+3);
	}


	unsigned int genStoreCode() {return getNewRegister();};

	unsigned int genCode() {
		return getNewRegister();
    }

};


class SwitchStatement : public Statement {
private:
	Expression *expression;
	Statement *statement;

public:
	SwitchStatement(Expression *expression, Statement *statement) {
		this->expression = expression;
		this->statement = statement;
	}

	void dump(int indent) {
		label(indent++, "SwitchStatement\n");
		expression->dump(indent++);
		statement->dump(indent);
	}

	unsigned int genCode() { return getNewRegister(); }

	unsigned int genStoreCode() {	return getNewRegister(); }
};



class CaseClauseStatement : public Statement {
private:
	Expression *expression;
	StatementList *stmtList;

public:
	CaseClauseStatement(vector<Statement*> *stmtList) {
		this->stmtList = new StatementList(stmtList);
	}
	CaseClauseStatement(Expression *expression, vector<Statement*> *stmtList) {
		this->expression = expression;
		this->stmtList = new StatementList(stmtList);
	}

	void dump(int indent) {
		label(indent++, "CaseClauseStatement\n");
		if(expression){
			expression->dump(indent++);
		}
		stmtList->dump(indent);
	}

	unsigned int genCode() { return getNewRegister(); }

	unsigned int genStoreCode() {	return getNewRegister(); }
};

class CaseBlockStatement : public Statement {
private:
    vector<Statement*> *caseClauses;
public:
    CaseBlockStatement(vector<Statement*> *caseClauses) {
        this->caseClauses = caseClauses;
    };

    void dump(int indent) {
        label(indent, "CaseBlockStatement\n");

        if(caseClauses != NULL) {
            for (vector<Statement*>::iterator iter = caseClauses->begin(); iter != caseClauses->end(); ++iter)
                (*iter)->dump(indent+1);
        }
    }

    unsigned int genCode() {
        return getNewRegister();
    }

	unsigned int genStoreCode() {
		return global_var;
	};

};

class FunctionDeclaration : public Statement {
private:
	Expression* bindingIdentifier;
	vector<Expression*>* formalParameters;
	vector<Statement*>* functionBody;
public:
	FunctionDeclaration(Expression* bindingIdentifier, vector<Expression*>* formalParameters, vector<Statement*>* functionBody) {
		this->bindingIdentifier = bindingIdentifier;
		this->formalParameters = formalParameters;
		this->functionBody = functionBody;
	}

	void dump(int indent) {
		label(indent++, "FunctionDeclaration\n");
		if (bindingIdentifier != NULL) {
			bindingIdentifier->dump(indent);
		}
		label(indent, "FormalParameters\n");
		for (vector<Expression*>::iterator iter = formalParameters->begin(); iter != formalParameters->end(); ++iter) {
			(*iter)->dump(indent + 1);
		}
		label(indent, "FunctionBody\n");
		for (vector<Statement*>::iterator iter = functionBody->begin(); iter != functionBody->end(); ++iter) {
			(*iter)->dump(indent + 1);
		}
	}

	unsigned int genCode() {
		IdentifierExpression* functionName = dynamic_cast<IdentifierExpression*>(bindingIdentifier);
		std::string functionDeclaration = std::string("ESValue* " + functionName->getReferencedName() + "(");
		for (vector<Expression*>::iterator iter = formalParameters->begin(); iter != formalParameters->end(); ++iter) {
			functionDeclaration = functionDeclaration + dynamic_cast<IdentifierExpression*>(*iter)->getReferencedName() + ",";
		}
		functionDefinitions.push_back(functionDeclaration.substr(0, functionDeclaration.size()-1) + ") {");


		codeScopeDepth++;
		for (vector<Statement*>::iterator iter = functionBody->begin(); iter != functionBody->end(); ++iter) {
			(*iter)->genCode();
		}
		std::vector<std::string> body = codeScope[codeScopeDepth];
		for (std::vector<std::string>::iterator iter = body.begin(); iter != body.end(); ++iter) {
			// get a char pointer out of the string
			std::string s = (*iter);
			char* w = new char[s.size() + 1];
			std::copy(s.begin(), s.end(), w);
			w[s.size()] = '\0';
			emit(w);
		}
		codeScopeDepth--;

		// TODO this code should go into function calling......
//		unsigned int reg = getNewRegister();
//		emit("\tESValue* r%d = %s();", reg, functionName->getReferencedName().c_str());

		functionDefinitions.insert(functionDefinitions.end(), body.begin(), body.end());
		functionDefinitions.push_back("}");
		return getNewRegister();
	}

	unsigned int genStoreCode() {
		return getNewRegister();
	};
};

class AnonymousFunctionDeclaration : public Statement {
private:
	vector<Expression*>* formalParameters;
	vector<Statement*>* functionBody;
public:
	AnonymousFunctionDeclaration(vector<Expression*>* formalParameters, vector<Statement*>* functionBody) {
		this->formalParameters = formalParameters;
		this->functionBody = functionBody;
	}

	void dump(int indent) {
		label(indent++, "FunctionDeclaration\n");
		label(indent, "FormalParameters\n");
		for (vector<Expression*>::iterator iter = formalParameters->begin(); iter != formalParameters->end(); ++iter) {
			(*iter)->dump(indent + 1);
		}
		label(indent, "FunctionBody\n");
		for (vector<Statement*>::iterator iter = functionBody->begin(); iter != functionBody->end(); ++iter) {
			(*iter)->dump(indent + 1);
		}
	}

	unsigned int genCode() {
		// TODO parameters
		codeScopeDepth++;
		for (vector<Statement*>::iterator iter = functionBody->begin(); iter != functionBody->end(); ++iter) {
			(*iter)->genCode();
		}
		std::vector<std::string> body = codeScope[codeScopeDepth];
		for (std::vector<std::string>::iterator iter = body.begin(); iter != body.end(); ++iter) {
			// get a char pointer out of the string
			std::string s = (*iter);
			char* w = new char[s.size() + 1];
			std::copy(s.begin(), s.end(), w);
			w[s.size()] = '\0';
			emit(w);
		}
		codeScopeDepth--;

		functionDefinitions.insert(functionDefinitions.end(), body.begin(), body.end());
		functionDefinitions.push_back("}");
		return getNewRegister();
	}

	unsigned int genStoreCode() {
		return getNewRegister();
	};
};
