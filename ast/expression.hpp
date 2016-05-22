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
};

class DecimalIntegerLiteralExpression:public Expression{
private:
    int value;
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

	void GenStoreCode(FILE* file) 	{		
		emit(file, "JSValue* r%d = new Reference(env, \"%s\")", global_var, this->getReferencedName().c_str());	
		++global_var;
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
	void GenStoreCode(FILE* file) {
		emit(file, "JSValue* r%d = new Reference(env, \"%s\")", global_var, this->getValue().c_str());	
		++global_var;
	};
	
};

class AssignmentExpression:public Expression, Reference {
private:
    Expression *lhs, *rhs;
    int lhsReg, rhsReg;
public:
    AssignmentExpression(Expression *lhs, Expression *rhs) {
        this->lhs = lhs;
        this->rhs = rhs;
    };

    void dump(int indent) {
        label(indent, "AssignmentExpression\n");
        lhs->dump(indent + 1, "lhs");
        rhs->dump(indent + 1, "rhs");
    }
    
    

    /**
     * Returns the base value component of the reference
     * TODO: This method will need to be expanded for all possible types... maybe?
     */
    ESValue* getBase() {
    	

        // attempt to cast to a string
        StringLiteralExpression* stringLiteralExpression = dynamic_cast<StringLiteralExpression*>(rhs);
        if (stringLiteralExpression) {
            return new String(stringLiteralExpression->getValue());
        }

        // attempt to cast to an int
        DecimalIntegerLiteralExpression* decimalIntegerLiteralExpression = dynamic_cast<DecimalIntegerLiteralExpression*>(rhs);
        if (decimalIntegerLiteralExpression) {
            return new Number(decimalIntegerLiteralExpression->getValue());
        }

        // attempt to cast to a double
        DecimalLiteralExpression* decimalLiteralExpression = dynamic_cast<DecimalLiteralExpression*>(rhs);
        if (decimalLiteralExpression) {
            return new Number(decimalLiteralExpression->getValue());
        }

        // ??? fail!
        return new Undefined();
    }

    /**
     * Returns the referenced name component of the reference.
     */
    std::string getReferencedName() {
        IdentifierExpression *identifier = dynamic_cast<IdentifierExpression *> (lhs);
        if (identifier != NULL) {
            return identifier->getReferencedName();
        }
        return NULL;
    }

    /**
     * Returns the strict reference flag component of the reference.
     */
    bool isStrictReference() {
        return false;
    }

    /**
     * Returns true if Type(base) is Boolean, String, Symbol, or Number.
     */
    bool hasPrimitiveBase() {
        return getBase()->isPrimitive();
    }

    /**
     * Returns true if either the base value is an object or hasPrimitiveBase() is true; otherwise returns false.
     */
    bool isPropertyReference() {
        if (getBase()->getType() == object || getBase()->isPrimitive()) {
            return true;
        }
        return false;
    }

    /**
     * Returns true if the base value is undefined and false otherwise.
     */
    bool isUnresolvableReference() {
        return getBase()->getType() == undefined;
    }

    /**
     * Returns true if this reference has a thisValue component
     */
    bool isSuperReference() {
        return false;
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

   
    void GenStoreCode(FILE* file) 	{
    	if (!isUnresolvableReference()) {
    		setLHSRegister();
			lhs->GenStoreCode(file);
			setRHSRegister();
			rhs->GenStoreCode(file);
			GenCode(file);
		}		
	}
   
    
    void GenCode(FILE* file) {
		emit(file, "JSValue* r%d = Assign(r%d, r%d)", global_var, lhsReg, rhsReg);	
		++global_var;
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

    
    void GenCode(FILE* file) 	{
		
	}
	
	void GenStoreCode(FILE* file) {};

};

/* See ECMA Specifications http://www.ecma-international.org/ecma-262/6.0/#sec-unary-operators */

class UnaryExpression : public Expression, Reference {


private:
	 char operand; 
	 Expression *unaryExpression;
	 int opReg;
	 
public:
	UnaryExpression(char operand, Expression *unaryExpression) {
		this->operand = operand;
		this->unaryExpression = unaryExpression;
	};
	
	ESValue* getBase() {
    	

        // attempt to cast to a string
        StringLiteralExpression* stringLiteralExpression = dynamic_cast<StringLiteralExpression*>(unaryExpression);
        if (stringLiteralExpression) {
            return new String(stringLiteralExpression->getValue());
        }

        // attempt to cast to an int
        DecimalIntegerLiteralExpression* decimalIntegerLiteralExpression = dynamic_cast<DecimalIntegerLiteralExpression*>(unaryExpression);
        if (decimalIntegerLiteralExpression) {
            return new Number(decimalIntegerLiteralExpression->getValue());
        }

        // attempt to cast to a double
        DecimalLiteralExpression* decimalLiteralExpression = dynamic_cast<DecimalLiteralExpression*>(unaryExpression);
        if (decimalLiteralExpression) {
            return new Number(decimalLiteralExpression->getValue());
        }

        // ??? fail!
        return new Undefined();
    }

    /**
     * Returns the referenced name component of the reference.
     */
    std::string getReferencedName() {
        IdentifierExpression *identifier = dynamic_cast<IdentifierExpression *> (unaryExpression);
        if (identifier != NULL) {
            return identifier->getReferencedName();
        }
        return NULL;
    }

    /**
     * Returns the strict reference flag component of the reference.
     */
    bool isStrictReference() {
        return false;
    }

    /**
     * Returns true if Type(base) is Boolean, String, Symbol, or Number.
     */
    bool hasPrimitiveBase() {
        return getBase()->isPrimitive();
    }

    /**
     * Returns true if either the base value is an object or hasPrimitiveBase() is true; otherwise returns false.
     */
    bool isPropertyReference() {
        if (getBase()->getType() == object || getBase()->isPrimitive()) {
            return true;
        }
        return false;
    }

    /**
     * Returns true if the base value is undefined and false otherwise.
     */
    bool isUnresolvableReference() {
        return getBase()->getType() == undefined;
    }

    /**
     * Returns true if this reference has a thisValue component
     */
    bool isSuperReference() {
        return false;
    }

	
	 void dump(int indent) {
        label(indent, "UnaryExpression\n");
        indent++;
        char lab[12] = "Operator: ";
        strcat(lab, &operand);
        strcat(lab, "\n");
        label(indent, lab);
        label(++indent, "Value\n");
      	unaryExpression->dump(++indent);
		
    }
    
    /**
     * Sets the operator reference register
     */
     void setLHSRegister() {
     	opReg = global_var;
     	++global_var;
      }
     
    
    void GenCode(FILE* file) 	{		
   
	}
	
	
	void GenStoreCode(FILE* file) {
		emit(file, "JSValue* r%d = new Reference(operand, \"%c\")", ++global_var, operand);
		setLHSRegister();
		unaryExpression->GenStoreCode(file);
		emit(file, "JSValue* r%d = Assign(r%d, r%d)", global_var, opReg, opReg+1);

	};
	
	

};