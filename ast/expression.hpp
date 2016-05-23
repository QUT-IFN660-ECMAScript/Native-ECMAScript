#pragma once
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include "node.hpp"
#include <string> 
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <sstream>

#include "../type/type.hpp"

using namespace std;
extern int global_var;

inline unsigned int getNewRegister() {
	return global_var++;
}

class Expression:public Node{
public:
	virtual unsigned int genCode(FILE* file) = 0;
	virtual unsigned int genStoreCode(FILE* file)=0;
};

class DecimalIntegerLiteralExpression:public Expression{
private:
    int value;
    int refID;
public:
    DecimalIntegerLiteralExpression(int value){
        this->value = value;
    };

    int getValue() {
        return value;
    }

    void dump(int indent){
        label(indent, "IntegerLiteralExpression: %d\n", value);
    }
    	  
    unsigned int genCode(FILE* file) {
        return getNewRegister();
    }
	
	unsigned int genStoreCode(FILE* file) {
		unsigned int registerNumber = getNewRegister();		
		emit(file, "\tESValue* r%d = new Number(%d);", registerNumber, this->getValue());
		return registerNumber;
	 };
};

class DecimalLiteralExpression:public Expression{
private:
    double value;
    
public:
    DecimalLiteralExpression(double value){
        this->value = value;
    };

    double getValue() {
        return value;
    }
    
    void dump(int indent){
        label(indent, "IntegerLiteralExpression: %d\n", value);
    }

    unsigned int genCode(FILE* file) {
        return getNewRegister();
    }
	
	unsigned int genStoreCode(FILE* file) {
		unsigned int registerNumber = getNewRegister();
		emit(file, "\tESValue* r%d = new Reference(env, \"%d\");", registerNumber, this->getValue());
		return registerNumber;
	};
};

class IdentifierExpression:public Expression{
private:
    std::string name;
    Reference* reference;
    int refID;
public:
    IdentifierExpression(std::string name){
        this->name = name;
        this->reference = NULL;
    };

    std::string getReferencedName() {
        return name;
    }
    

    void dump(int indent){
        label(indent, "IdentifierExpression: %s\n", name.c_str());
    }

	unsigned int genCode(FILE* file) {
		return getNewRegister();
	}

	unsigned int genStoreCode(FILE* file) 	{
		unsigned int registerNumber = getNewRegister();
		emit(file, "\tESValue* r%d = new Reference(new String(\"%s\"));", registerNumber, this->getReferencedName().c_str());
		return registerNumber;
	}
};

class StringLiteralExpression: public Expression {
private:
	std::string val;
	const char* result;
public:
	StringLiteralExpression(const char* val) {
		this->val = std::string(val);
		this->result = val;
	};

    std::string getValue() {
        return val;
    }
    
    int getIntValue() {
		unsigned uintVar;
	  	std::istringstream in(val);
	   	in >> uintVar;
	   	return uintVar;
		
    }
    
	void dump(int indent) {
		label(indent, "StringLiteralExpression: %s\n", val.c_str());
	}

    unsigned int genCode(FILE* file) 	{
        return getNewRegister();
    }

	unsigned int genStoreCode(FILE* file) {
		unsigned int registerNumber = getNewRegister();
		emit(file, "\tESValue* r%d = new Reference(env, \"%s\");", registerNumber, this->getValue().c_str());
		return registerNumber;
	};
};

class AssignmentExpression:public Expression {
private:
    Expression *lhs, *rhs;
    int lhsReg, rhsReg;
public:
    AssignmentExpression(Expression *lhs, Expression *rhs) {
        this->lhs = lhs;
        this->rhs = rhs;
    };
    
    AssignmentExpression() {};

    AssignmentExpression(Expression* expression){
        this->lhs = expression;
    }

    void dump(int indent) {
        label(indent, "AssignmentExpression\n");
        lhs->dump(indent + 1, "lhs");
        if(rhs != NULL){
            rhs->dump(indent + 1, "rhs");
        }
    }

    unsigned int genStoreCode(FILE* file) 	{
    	
    	unsigned int lhsRegisterNumber = lhs->genStoreCode(file);
		unsigned int rhsRegisterNumber = rhs->genStoreCode(file);
		unsigned int registerNumber = getNewRegister();	

		emit(file, "\tESValue* r%d = Core::assign(r%d, r%d);", registerNumber, lhsRegisterNumber, rhsRegisterNumber);
		return registerNumber;
	}
   
    
    unsigned int genCode(FILE* file) {
        return getNewRegister();
    }
};

class ObjectLiteralExpression : public Expression {
private:
    vector<Expression*> *propertyDefinitionList;
public:
    //No parameter constructor
    ObjectLiteralExpression(){};
    ObjectLiteralExpression(vector<Expression*> *propertyDefinitionList) {
        this->propertyDefinitionList = propertyDefinitionList;
    };

    void dump(int indent) {
        label(indent, "ObjectLiteralExpression\n");

        if(propertyDefinitionList != NULL) {
            for (vector<Expression*>::iterator iter = propertyDefinitionList->begin(); iter != propertyDefinitionList->end(); ++iter)
                (*iter)->dump(indent+1);
        }
    }

    unsigned int genCode(FILE* file) {
        return getNewRegister();
    }
	
	unsigned int genStoreCode(FILE* file) {
		return global_var;
	};

};

class PropertyDefinitionExpression : public Expression {
private:
    Expression *key;
    Expression *value;

public:
    PropertyDefinitionExpression(Expression *key, Expression *value) {
        this->key = key;
        this->value = value;
    };

    void dump(int indent) {
        label(indent, "PropertyDefinitionExpression\n");
        indent++;
        label(indent, "Key\n");
        key->dump(indent + 1);
        label(indent, "Value\n");
        if (value != NULL) {
            value->dump(indent + 1);
        } else {
            label(indent + 1, "[UNDEFINED]\n");
        }
    }

    unsigned int genCode(FILE* file) {
        return getNewRegister();
    }
	
	unsigned int genStoreCode(FILE* file) {
		return global_var;
	};
	

};

class LiteralPropertyNameExpression : public Expression {
    Expression *literalExpression;
public:
    LiteralPropertyNameExpression(Expression *literalExpression) {
        this->literalExpression = literalExpression;
    };

    void dump (int indent) {
        label(indent, "LiteralPropertyNameExpression\n");
        indent++;
        literalExpression->dump(indent);
    }

    unsigned int genCode(FILE* file) {
        return getNewRegister();
    }
	
	unsigned int genStoreCode(FILE* file) {
		return global_var;
	};
};

class ComputedPropertyNameExpression : public Expression {
private:
    Expression *computedExpression;
public:
    ComputedPropertyNameExpression(Expression *computedExpression) {
        this->computedExpression = computedExpression;
    };

    void dump(int indent) {
        label(indent, "ComputedPropertyNameExpression\n");
        indent++;
        computedExpression->dump(indent);
    }

    unsigned int genCode(FILE* file) 	{
        return getNewRegister();
    }
	
	unsigned int genStoreCode(FILE* file) {
		return global_var;
	};

};

/* See ECMA Specifications http://www.ecma-international.org/ecma-262/6.0/#sec-unary-operators */

class UnaryExpression : public Expression {


private:
	 char operand; 
	 Expression *unaryExpression;
	 int opRegL, opRegR;
	 
public:
	UnaryExpression(char operand, Expression *unaryExpression) {
		this->operand = operand;
		this->unaryExpression = unaryExpression;
	};
	
	
	
	 void dump(int indent) {
        label(indent, "UnaryExpression\n");
        indent++;
		/* need to handle different unary operators */
        label(indent, "Operator: +\n");
        label(++indent, "Value\n");
      	unaryExpression->dump(++indent);
		
    }
    
    
    unsigned int genCode(FILE* file) 	{
        return getNewRegister();
    }
	
	
	unsigned int genStoreCode(FILE* file) {
		//emit(file, "ESValue* r%d = new Reference(operand, \"%c\")", global_var, operand);
		//unaryExpression->genStoreCode(file);
		//emit(file, "ESValue* r%d = Plus(r%d, r%d)", global_var, this->getRegisterNumber(), this->getRegisterNumber());
		return global_var;
	};

};

class PlusAditiveExpression : public Expression {

private:
	char operand; 
	Expression *lhs;
	Expression *rhs;
	int opRegL, opRegR;
	 
	public:
	PlusAditiveExpression(Expression *lhs,  Expression *rhs) {
		this->lhs = lhs;
		this->rhs = rhs;
	};
	 
	 
	void dump(int indent) {
		label(indent, "PlusAdditiveExpression\n");
		lhs->dump(++indent);
		
		rhs->dump(indent);   
    }
    
	unsigned int genCode(FILE* file) 	{
        return getNewRegister();
    }
	
	
	unsigned int genStoreCode(FILE* file) {
		unsigned int lhsRegisterNumber = lhs->genStoreCode(file);
		unsigned int rhsRegisterNumber =  rhs->genStoreCode(file);
		unsigned int registerNumber = getNewRegister();	
		emit(file, "\tESValue* r%d = Core::plus(r%d, r%d);", registerNumber, lhsRegisterNumber, rhsRegisterNumber );
		return registerNumber;
		
	};
};

class Arguments : public Expression {
private:
    std::vector<AssignmentExpression*>* argumentList;
public:
    Arguments(vector<AssignmentExpression*>* argumentList){
        this->argumentList = argumentList;
    };

    void dump(int indent) {
        label(indent, "Arguments\n");
        for (vector<AssignmentExpression*>::iterator iter = argumentList->begin(); iter != argumentList->end(); ++iter)
          (*iter)->dump(indent+1);
    }

  unsigned int genCode(FILE *file) {
      return getNewRegister();
    }
};


class CallExpression : public Expression {
private:
    Expression* expression;
    Arguments* arguments;
public:
    CallExpression(Expression *expression) {
        this->expression = expression;
    };

    void dump(int indent) {
        label(indent, "CallExpression\n");
        indent++;
        expression->dump(indent);
        if(arguments != NULL) {
            arguments->dump(indent+1);
        }
    }


    unsigned int genCode(FILE *file) {
        return getNewRegister();
    }
};

// TODO: Refactor this into each binary expression class...
class BinaryExpression : public Expression {
private:
    Expression* lhs;
    Expression* rhs;
    char op;
public:
    BinaryExpression(Expression* lhs, Expression* rhs, char op) {
        this->lhs = lhs;
        this->rhs = rhs;

        this->op = op;
    };

    unsigned int genStoreCode(FILE* file) {

        //rhs must genCode() first, otherwise, the output will be wrong
        unsigned int rhsRegisterNumber = rhs->genCode(file);
        unsigned int lhsRegisterNumber = lhs->genCode(file);
        unsigned int registerNumber = getNewRegister();

        switch(op) {
            case '+':
                emit(file, "ESValue* r%d = Core::plus(r%d, r%d);", registerNumber, lhsRegisterNumber, rhsRegisterNumber);
                break;
            case '-':
                emit(file, "ESValue* r%d = Core::subtract(r%d, r%d);", registerNumber, lhsRegisterNumber, rhsRegisterNumber);
                break;
            case '*':
                emit(file, "ESValue* r%d = Core::multiply(r%d, r%d);", registerNumber, lhsRegisterNumber, rhsRegisterNumber);
                break;
            case '/':
                emit(file, "ESValue* r%d = Core::divide(r%d, r%d);", registerNumber, lhsRegisterNumber, rhsRegisterNumber);
                break;
            case '%':
                emit(file, "ESValue* r%d = Core::modulo(r%d, r%d);", registerNumber, lhsRegisterNumber, rhsRegisterNumber);
                break;
        }

        return registerNumber;
    }

    void dump(int indent) {
        label(indent, "BinaryExpression\n");
        label(indent + 1, "op: %c\n", op);
        lhs->dump(indent + 1, "lhs");
        if(rhs != NULL){
            rhs->dump(indent + 1, "rhs");
        }
    }
};