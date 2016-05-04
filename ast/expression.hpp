#pragma once
#include <cstdarg>
#include <cstdio>
#include "node.hpp"

using namespace std;

class Expression:public Node{
};



class IntegerLiteralExpression:public Expression{
private:
    int value;
public:
    IntegerLiteralExpression(int value){
        this->value = value;
    };
    void dump(int indent){
        label(indent, "IntegerLiteralExpression: %d\n", value);
    }

    bool resolveName(LexicalScope* scope) {
        return true; //not implemented
    }
};

class IdentifierExpression:public Expression{
private:
    char* name;
    Declaration* declaration;//Store the pointer where the identifer is declared
public:
    IdentifierExpression(char *name){
        this->name = name;
    };
    void dump(int indent){
        label(indent, "IdentifierExpression: %s\n", name);
    }

    bool resolveName(LexicalScope* scope) {
        return true; //not implemented
    }

    char* getIdentifierName() {
        return name;
    }
};

class StringLiteralExpression: public Expression {
private:
	char* val;
public:
	StringLiteralExpression(char* val) {
		this->val = val;
	};
	void dump(int indent) {
		label(indent, "StringLiteralExpression: %s\n", val);
	}

    bool resolveName(LexicalScope* scope) {
        return true; //not implemented
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

    bool resolveName(LexicalScope* scope) {
        return true; //not implemented
    }
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
        if(value != NULL) {
            value->dump(indent + 1);
        } else {
            label(indent + 1, "[UNDEFINED]\n");
        }
    }

    bool resolveName(LexicalScope* scope) {
        return true; //not implemented
    }
};

class LiteralPropertyNameExpression : public Expression {
private:
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

    bool resolveName(LexicalScope* scope) {
        return true; //not implemented
    }
};

class ComputedPropertyNameExpression : public Expression {
private:
    Expression *computedExpression;
public:
    ComputedPropertyNameExpression (Expression *computedExpression) {
        this->computedExpression = computedExpression;
    };

    void dump(int indent) {
        label(indent, "ComputedPropertyNameExpression\n");
        indent++;
        computedExpression->dump(indent);
    }

    bool resolveName(LexicalScope* scope) {
        return true; //not implemented
    }
};

class AssignmentExpression:public Expression, public Declaration{
private:
    Expression *lhs, *rhs;
public:
    AssignmentExpression(Expression *lhs, Expression *rhs){
        this->lhs = lhs;
        this->rhs = rhs;
    };

    void dump(int indent){
        IdentifierExpression* iden = dynamic_cast<IdentifierExpression*>(lhs);
        if(iden) {
            label(indent, "%p: VariableDeclaration %s\n", (Declaration*)this, iden->getIdentifierName());
        }
        label(indent, "AssignmentExpression\n");
        lhs->dump(indent+1, "lhs");
        rhs->dump(indent+1, "rhs");
    }

    std::string getName() {
        // static_cast<IdentifierExpression*>(lhs)->getIdentifierName();
        IdentifierExpression* iden = dynamic_cast<IdentifierExpression*>(lhs);
        if(iden) {
            return iden->getIdentifierName();
        }
        return "";
    }

    bool resolveName(LexicalScope* scope) {
        return lhs->resolveName(scope) && rhs->resolveName(scope);
    }
};