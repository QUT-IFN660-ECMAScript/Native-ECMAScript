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


class Expression:public Node{
public:	 
	virtual void GenCode(FILE* file) = 0;
	virtual void GenStoreCode(FILE* file)=0;
	virtual void setRHSRegister() = 0;
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
    
	void setRefID() {
		refID = global_var;
	}
	
	int getRefID() {
		return refID;
	}
	
	void setRHSRegister() {}
  
    void GenCode(FILE* file)
	{
		
	}
	
	void GenStoreCode(FILE* file) {		
		emit(file, "JSValue* r%d = new Reference(env, \"%d\")", global_var, this->getValue());
		++global_var;
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
    
	void setRHSRegister() {}

    void dump(int indent){
        label(indent, "IntegerLiteralExpression: %d\n", value);
    }
  
    void GenCode(FILE* file)
	{
		
	}
	
	void GenStoreCode(FILE* file) {
		emit(file, "JSValue* r%d = new Reference(env, \"%d\")", global_var, this->getValue());
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
   
	void setRefID() {
		refID = global_var;
		global_var++;
	}

	void setRHSRegister() {}
	
	int getRefID() {
		return refID;
	}
    
	void GenCode(FILE* file) {
		
	}

	void GenStoreCode(FILE* file) 	{		
		setRefID();
		emit(file, "JSValue* r%d = new Reference(env, \"%s\")", this->getRefID(), this->getReferencedName().c_str());	
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
    
    void setRHSRegister() {}

	void dump(int indent) {
		label(indent, "StringLiteralExpression: %s\n", val.c_str());
	}
   
    
    void GenCode(FILE* file) 	{
		
	}
	void GenStoreCode(FILE* file) {
		emit(file, "JSValue* r%d = new Reference(env, \"%s\")", global_var, this->getValue().c_str());	
		++global_var;
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
  
    /**
     * Sets the lhs reference register
     */
     void setLHSRegister() {
     	lhsReg = global_var;
      }
     
     /**
     * Sets the rhs reference register
     */
     void setRHSRegister() {
     	rhsReg = global_var;
     }
     
     int getLHSRegister() {
     	return lhsReg;
     }
     
     int getRHSRegister() {
     	return rhsReg;
     }

   
    void GenStoreCode(FILE* file) 	{
    	setLHSRegister();
		lhs->GenStoreCode(file);
		setRHSRegister();
		rhs->GenStoreCode(file);		
		GenCode(file);
	}
   
    
    void GenCode(FILE* file) {      		
		emit(file, "JSValue* r%d = Assign(r%d, r%d)", global_var, this->getLHSRegister(), this->getRHSRegister());	
		setLHSRegister();		
		++global_var;
		setRHSRegister(); 
		
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

	void setRHSRegister() {}
   
    
    void GenCode(FILE* file)
	{
		
	}
	
	void GenStoreCode(FILE* file) {};
	
	

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

    void setRHSRegister() {}
    
    void GenCode(FILE* file)
	{
		
	}
	
	void GenStoreCode(FILE* file) {};
	
	
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

    void setRHSRegister() {}
    
    void GenCode(FILE* file)
	{
		
	}
	
	void GenStoreCode(FILE* file) {};
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

    void setRHSRegister() {}
    
    void GenCode(FILE* file) 	{
		
	}
	
	void GenStoreCode(FILE* file) {};

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
    
    /**
     * Sets the operator reference register
     */
     void setLHSRegister() {
     	opRegL = global_var;
     	++global_var;
      }
      
      int getLHSRegister() {
      	return opRegL;
      }
      
      /**
     * Sets the assign reference register
     */
	void setRHSRegister() {
     	opRegR = global_var;
     //	++global_var;
	}
      
	int getRHSRegister() {
		return opRegR;
	}
     
    
    void GenCode(FILE* file) 	{		
   
	}
	
	
	void GenStoreCode(FILE* file) {
		emit(file, "JSValue* r%d = new Reference(operand, \"%c\")", global_var, operand);		
		setLHSRegister();		
		setRHSRegister();
		unaryExpression->GenStoreCode(file);						
		emit(file, "JSValue* r%d = Plus(r%d, r%d)", global_var, this->getLHSRegister(), this->getRHSRegister());

	};

};

class AdditiveExpression : public Expression {

private:
	char operand; 
	Expression *additiveExpression;
	Expression *unaryExpression;
	int opRegL, opRegR;
	 
	public:
	AdditiveExpression(Expression *additiveExpression, char operand, Expression *unaryExpression) {
		this->additiveExpression = additiveExpression;
		this->unaryExpression = unaryExpression;
		this->operand = operand;
	};
	 
	 
	void dump(int indent) {
		additiveExpression->dump(indent);
		unaryExpression->dump(indent);   
    }
    
    void setLHSRegister() {
		opRegL = global_var;
	}
      
	int getLHSRegister() {
		return opRegL;
	}
	
	void setRHSRegister() {
		opRegR = global_var;
	}
      
	int getRHSRegister() {
		return opRegR;
	}

	

	void GenCode(FILE* file) 	{	
 
	}
	
	
	void GenStoreCode(FILE* file) {	
		setLHSRegister();
		additiveExpression->GenStoreCode(file);					
		unaryExpression->GenStoreCode(file);
		setRHSRegister();	
		emit(file, "JSValue* r%d = Plus(r%d, r%d)", ++global_var, this->getLHSRegister(), this->getRHSRegister() );	
		global_var++;
		
	};


};