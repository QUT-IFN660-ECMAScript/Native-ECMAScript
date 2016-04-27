#pragma once
#include <cstdarg>
#include <cstdio>
#include "node.hpp"

using namespace std;

class Expression:public Node{
};

class AssignmentExpression:public Expression{
private:
    Expression *lhs, *rhs;
public:
    AssignmentExpression(Expression *lhs, Expression *rhs){
        this->lhs = lhs;
        this->rhs = rhs;
    };
    void dump(int indent){
        label(indent, "AssignmentExpression\n");
        lhs->dump(indent+1, "lhs");
        rhs->dump(indent+1, "rhs");
    }
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
};

class IdentifierExpression:public Expression{
private:
    char* name;
public:
    IdentifierExpression(char *name){
        this->name = name;
    };
    void dump(int indent){
        label(indent, "IdentifierExpression: %s\n", name);
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
};