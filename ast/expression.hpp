#pragma once
#include <cstdarg>
#include <cstdio>
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
	virtual void GenCode(FILE* file) = 0;
	virtual unsigned int GenStoreCode(FILE* file)=0;
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
    	  
    void GenCode(FILE* file)
	{
		
	}
	
	unsigned int GenStoreCode(FILE* file) {
		unsigned int registerNumber = getNewRegister();		
		emit(file, "JSValue* r%d = new Reference(env, \"%d\")", registerNumber, this->getValue());
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
  
    void GenCode(FILE* file)
	{
		
	}
	
	unsigned int GenStoreCode(FILE* file) {
		unsigned int registerNumber = getNewRegister();
		emit(file, "JSValue* r%d = new Reference(env, \"%d\")", registerNumber, this->getValue());
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
   
	void GenCode(FILE* file) {
		
	}

	unsigned int GenStoreCode(FILE* file) 	{		
		unsigned int registerNumber = getNewRegister();
		emit(file, "JSValue* r%d = new Reference(env, \"%s\")", registerNumber, this->getReferencedName().c_str());	
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
   
    
    void GenCode(FILE* file) 	{
		
	}
	unsigned int GenStoreCode(FILE* file) {
		unsigned int registerNumber = getNewRegister();
		emit(file, "JSValue* r%d = new Reference(env, \"%s\")", registerNumber, this->getValue().c_str());	
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

    void dump(int indent) {
        label(indent, "AssignmentExpression\n");
        lhs->dump(indent + 1, "lhs");
        rhs->dump(indent + 1, "rhs");
    }
    
    unsigned int GenStoreCode(FILE* file) 	{
    	
    	unsigned int lhsRegisterNumber = lhs->GenStoreCode(file);
		unsigned int rhsRegisterNumber = rhs->GenStoreCode(file);
		unsigned int registerNumber = getNewRegister();	

		emit(file, "JSValue* r%d = Assign(r%d, r%d)", registerNumber, lhsRegisterNumber, rhsRegisterNumber);		
		return registerNumber;
	}
   
    
    void GenCode(FILE* file) {      			
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

    void GenCode(FILE* file)
	{
		
	}
	
	unsigned int GenStoreCode(FILE* file) {
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

    void GenCode(FILE* file)
	{
		
	}
	
	unsigned int GenStoreCode(FILE* file) {
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

    void GenCode(FILE* file)
	{
		
	}
	
	unsigned int GenStoreCode(FILE* file) {
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

    void GenCode(FILE* file) 	{
		
	}
	
	unsigned int GenStoreCode(FILE* file) {
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
    
    
    void GenCode(FILE* file) 	{		
   
	}
	
	
	unsigned int GenStoreCode(FILE* file) {
		//emit(file, "JSValue* r%d = new Reference(operand, \"%c\")", global_var, operand);		
		//unaryExpression->GenStoreCode(file);						
		//emit(file, "JSValue* r%d = Plus(r%d, r%d)", global_var, this->getRegisterNumber(), this->getRegisterNumber());
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
    
	void GenCode(FILE* file) 	{	
 
	}
	
	
	unsigned int GenStoreCode(FILE* file) {	
		unsigned int lhsRegisterNumber = lhs->GenStoreCode(file);
		unsigned int rhsRegisterNumber =  rhs->GenStoreCode(file);
		unsigned int registerNumber = getNewRegister();	
		emit(file, "JSValue* r%d = Plus(r%d, r%d)", registerNumber, lhsRegisterNumber, rhsRegisterNumber );	
		return registerNumber;
		
	};


};