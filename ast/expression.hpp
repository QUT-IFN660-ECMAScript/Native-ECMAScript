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
