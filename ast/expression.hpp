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

    int getValue() {
        return value;
    }

    void dump(int indent){
        label(indent, "IntegerLiteralExpression: %d\n", value);
    }
    bool resolveNames(LexicalScope* scope) {
        return true;
    }
};

class IdentifierExpression:public Expression{
private:
    std::string name;
    Declaration* declaration;
public:
    IdentifierExpression(std::string name){
        this->name = name;
        this->declaration = NULL;
    };

    std::string getName() {
        return name;
    }

    void dump(int indent){
        label(indent, "IdentifierExpression: %s\n", name.c_str());
    }
    bool resolveNames(LexicalScope* scope) {
        if (scope != NULL) {
            declaration = scope->resolve(name);
        }
        if (declaration == NULL) {
            fprintf(stderr, "Error: Undeclared identifier: %s\n", name.c_str());
        }
        return declaration != NULL;
    }
};

class StringLiteralExpression: public Expression {
private:
	std::string val;
public:
	StringLiteralExpression(const char* val) {
		this->val = std::string(val);
	};
	void dump(int indent) {
		label(indent, "StringLiteralExpression: %s\n", val.c_str());
	}
    bool resolveNames(LexicalScope* scope) {
        return true;
    }
};

class AssignmentExpression:public Expression, Declaration {
private:
    Expression *lhs, *rhs;
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

    std::string getName() {
        IdentifierExpression *identifier = dynamic_cast<IdentifierExpression *> (lhs);
        if (identifier != NULL) {
            return identifier->getName();
        }
        return NULL;
    }

    bool resolveNames(LexicalScope *scope) {
        if (lhs && rhs) {
//        return true;
            IdentifierExpression *identifier = dynamic_cast<IdentifierExpression *> (lhs);
            if (identifier != NULL) {
                // not sure `this` in this context is what we are after, but whatever
                scope->addToSymbolTable(identifier->getName(), this);
            }
            return lhs->resolveNames(scope) && rhs->resolveNames(scope);
        }
        return false;
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

    bool resolveNames(LexicalScope* scope) {
        bool scoped = true;
        if (propertyDefinitionList) {
            for (vector<Expression *>::iterator it = propertyDefinitionList->begin();
                 it != propertyDefinitionList->end(); ++it) {
                if (*it) {
                    if (!(*it)->resolveNames(scope)) {
                        scoped = false;
                    }
                }
            }
            return scoped;
        }
        return false;
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
        if (value != NULL) {
            value->dump(indent + 1);
        } else {
            label(indent + 1, "[UNDEFINED]\n");
        }
    }

    bool resolveNames(LexicalScope *scope) {
        if (key && value) {
            return key->resolveNames(scope) && value->resolveNames(scope);
        }
        return false;
    };
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

    bool resolveNames(LexicalScope* scope) {
        if (literalExpression) {
            return literalExpression->resolveNames(scope);
        }
        return false;
    }
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

    bool resolveNames(LexicalScope *scope) {
        if (computedExpression) {
            return computedExpression->resolveNames(scope);
        }
        return false;
    }

};