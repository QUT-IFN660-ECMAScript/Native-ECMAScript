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
		if(stmts != NULL) {
			for (vector<Statement*>::iterator iter = stmts->begin(); iter != stmts->end(); ++iter)
                (*iter)->genCode();
		}
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

	unsigned int genCode() { return getNewRegister(); }

	unsigned int genStoreCode() { return getNewRegister(); };

	/* 
	 * This method is called by subclasses of LabelledStatement for file output of register operation
	 */
	unsigned int fileEmit(const char* type) {
		if(expr != NULL){
			expr->genStoreCode();
			unsigned int reg = getNewRegister();
			emit("\t%s r%d;", type, reg - 1);
		} else {
			emit("\t%s;", type);
			return getNewRegister();
		}
	}
};

class BreakStatement: public LabelledStatement {
private:
	Expression* expr;

public:
	BreakStatement(){
		this->expr = NULL;
	}
	BreakStatement(Expression* expr) : LabelledStatement(expr) {
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

	unsigned int genCode() {
		return LabelledStatement::fileEmit(LABEL_BREAK);
	}

	unsigned int genStoreCode() { return getNewRegister(); };
};


class ContinueStatement: public LabelledStatement {
private:
	Expression* expr;

public:
	ContinueStatement(){
		this->expr = NULL;
	}
	ContinueStatement(Expression* expr) : LabelledStatement(expr) {
		this->expr = expr;
	}

	void dump(int indent) {
		label(indent++, "ContinueStatement\n");
		if(this->expr != NULL){
			expr->dump(indent);
		} else {
			label(indent, "[Empty]\n");
		}
	}

	unsigned int genCode() {
		return LabelledStatement::fileEmit(LABEL_CONTINUE);
	}

	unsigned int genStoreCode() { return getNewRegister(); };
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
		//13.6.7 Runtime Semantics: Evaluation
		int regConditionalExpression = expression->genStoreCode();
		int regNum = getNewRegister();

		//Convert the conditional expression to boolean
		emit("\tBoolean r%d = TypeOps::toBoolean(r%d);", regNum, regConditionalExpression);

		if(elseStatement != NULL) {
			emit("//Simulate the jump if in assembly");
			emit("\tif(!r%d.getValue())", regNum);

			emit("\t\tgoto label_else_r%d;", regNum);

			emit("\t//the code is executed if the conditional expression is true}");

			//compiling .c file will throw error if not putting the code in {...}
			emit("\t{");
			statement->genCode();
			emit("\t}");

			emit("\tgoto label_end_if_r%d;", regNum);

			emit("label_else_r%d:", regNum);
			
			//compiling .c file will throw error if not putting the code in {...}
			emit("\t{");
			elseStatement->genCode();
			emit("\t}");


			emit("label_end_if_r%d:");

		} else {
			emit("//Simulate the jump if in assembly");
			emit("\tif(!r%d.getValue())", regNum);

			emit("\t\tgoto label_end_if_r%d;", regNum);

			emit("\t//the code is executed if the conditional expression is true}");
			statement->genCode();

			//emit the label
			emit("label_end_if_r%d:", regNum);
		}

		return regNum;
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
	Expression* getCaseExpression() {
		return expression;
	}

	void dump(int indent) {
		label(indent++, "CaseClauseStatement\n");
		if(expression){
			expression->dump(indent++);
		}
		stmtList->dump(indent);
	}

	unsigned int genCode() { 
		// emit("");
		unsigned int regNum = getNewRegister();
		emit("LABEL%d:", regNum);
		this->stmtList->genCode();
		return regNum;
	}

	unsigned int genStoreCode() {	return getNewRegister(); }
};

class CaseBlockStatement : public Statement {
private:
    vector<Statement*> *caseClauses;
	unsigned int endLabelNum;
	// vector<Expression*> *caseExpression;
	// vector<int> *labelRegNum;
	std::map<unsigned int, Expression*> caseLabelMap;
public:
    CaseBlockStatement(vector<Statement*> *caseClauses) {
        this->caseClauses = caseClauses;
        // this->caseExpression = new vector<Expression*>;
        // this->labelRegNum = new vector<int>;
    };

	void setEndLabelNum(unsigned int num) {
		this->endLabelNum = num;
	}
	// vector<Expression*>* getCaseExpression() {
	// 	return this->caseExpression;
	// }
	// vector<int>* getLabelRegNum() {
	// 	return this->labelRegNum;
	// }
	std::map<unsigned int, Expression*> getCaseLabelMap() {
		return this->caseLabelMap;
	}

    void dump(int indent) {
        label(indent, "CaseBlockStatement\n");

        if(caseClauses != NULL) {
            for (vector<Statement*>::iterator iter = caseClauses->begin(); iter != caseClauses->end(); ++iter)
                (*iter)->dump(indent+1);
        }
    }

    unsigned int genCode() {
        if(caseClauses != NULL) {
            for (vector<Statement*>::iterator iter = caseClauses->begin(); iter != caseClauses->end(); ++iter) {
				// unsigned int regNum = getNewRegister();
            	unsigned int labelRegNum = (*iter)->genCode();
				emit("\tgoto LABELEND%d;", endLabelNum);
				CaseClauseStatement *ccStmt = dynamic_cast<CaseClauseStatement*>((*iter));
				// this->caseExpression.push_back(ccStmt->getCaseExpression());
				caseLabelMap[labelRegNum] = ccStmt->getCaseExpression();
				caseLabelMap.insert(std::pair<unsigned int, Expression*>(labelRegNum, ccStmt->getCaseExpression()));
				
            }
        }
        return getNewRegister();
    }

	unsigned int genStoreCode() {
		return global_var;
	};

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

	unsigned int genCode() { 
		unsigned int reservedForStart = getNewRegister();
		unsigned int reservedForEnd = getNewRegister();
		emit("\tgoto LABEL%d;", reservedForStart);

		CaseBlockStatement *cbStmt = dynamic_cast<CaseBlockStatement*>(statement);
		cbStmt->setEndLabelNum(reservedForEnd);
		cbStmt->genCode();
		emit("LABEL%d:", reservedForStart);
		unsigned int switchRegNum = this->expression->genStoreCode();
		
		// emit("\t//These are values of case labels. (e.g. case 1:, case 2:, ...)");
		// vector<Expression*> *caseExpression = cbStmt->getCaseExpression();
		// for (vector<Expression*>::iterator iter = caseExpression->begin(); iter != caseExpression->end(); ++iter) {
		std::map<unsigned int, Expression*> caseLabelMap = cbStmt->getCaseLabelMap();
		for (std::map<unsigned int, Expression*>::iterator iter = caseLabelMap.begin(); iter != caseLabelMap.end(); ++iter) {

			// unsigned int caseLabelNum = (*iter)->genStoreCode();
			unsigned int caseLabelNum = (iter->second)->genStoreCode();
			// emit("\t//If these two have the same value, Core::zeroFlag will be true");
			emit("\tCore::compare(r%d, r%d);", switchRegNum, caseLabelNum);
			emit("\tif(Core::zeroFlag) goto LABEL%d;", iter->first);
			// if(r2 == 1) goto SWTLAB6;
		}

		emit("LABELEND%d:", reservedForEnd);


		return getNewRegister();
	}

	unsigned int genStoreCode() { return getNewRegister(); }
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
