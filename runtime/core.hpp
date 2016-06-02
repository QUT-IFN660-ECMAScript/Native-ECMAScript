//
// Created by Harry Scells on 8/05/2016.
//
#pragma once

#include "../type/type.hpp"
#include "../scope/reference.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>

enum Exception {
    ReferenceError,
    TypeError
};

extern ESObject* globalObj;

class Core {
public:
    /**
     * 12.7.3 The Addition operator ( + )
     * http://www.ecma-international.org/ecma-262/6.0/#sec-addition-operator-plus
     */
    static ESValue* plus(ESValue* lref, ESValue* rref) {

        Number* lnum = TypeOps::toNumber(lref);
        Number* rnum = TypeOps::toNumber(rref);

        // If either operand is NaN, the result is NaN.
        if (lnum->isNan()->getValue() || rnum->isNan()->getValue()) {
            return new NaN();
        }

        return new Number(lnum->getValue() + rnum->getValue());
    }

    /** 12.5.9.1 Runtime Semantics: Evaluation
     * The unary + operator converts its operand to Number type.
     */
    static ESValue* plus_u(ESValue* rref) {

        Number* rnum = TypeOps::toNumber(rref);

        // If operand is NaN, the result is NaN.
        if (rnum->isNan()->getValue()) {
            return new NaN();
        }

        return new Number(rnum->getValue());
    }

     /** 12.5.10.1 Runtime Semantics: Evaluation
     * The unary - operator converts its operand to Number type and then negates it. 
     * Negating +0 produces −0, and negating −0 produces +0.
     */
    static ESValue* subtract_u(ESValue* rref) {

        Number* rnum = TypeOps::toNumber(rref);

        // If operand is NaN, the result is NaN.
        if (rnum->isNan()->getValue()) {
            return new NaN();
        }

        return new Number(rnum->getValue());
    }


    /**
     * 12.7.4 The Subtraction Operator ( - )
     * http://www.ecma-international.org/ecma-262/6.0/#sec-subtraction-operator-minus
     */
    static ESValue* subtract(ESValue* lref, ESValue* rref) {

        Number* lnum = TypeOps::toNumber(lref);
        Number* rnum = TypeOps::toNumber(rref);

        // If either operand is NaN, the result is NaN.
        if (lnum->isNan()->getValue() || lnum->isNan()->getValue()) {
            return new NaN();
        }

        return new Number(lnum->getValue() - rnum->getValue());

    }

    /**
     * 12.6.3.1 Applying the * Operator
     * http://www.ecma-international.org/ecma-262/6.0/#sec-applying-the-mul-operator
     */
    static ESValue* multiply(ESValue* lref, ESValue* rref) {

        Number* lnum = TypeOps::toNumber(lref);
        Number* rnum = TypeOps::toNumber(rref);

        // If either operand is NaN, the result is NaN.
        if (lnum->isNan()->getValue() || lnum->isNan()->getValue()) {
            return new NaN();
        }

        return new Number(lnum->getValue() * rnum->getValue());

    }

    /**
     * 12.6.3.2 Applying the / Operator
     * http://www.ecma-international.org/ecma-262/6.0/#sec-applying-the-div-operator
     */
    static ESValue* divide(ESValue* lref, ESValue* rref) {

        Number* lnum = TypeOps::toNumber(lref);
        Number* rnum = TypeOps::toNumber(rref);

        // If either operand is NaN, the result is NaN.
        if (lnum->isNan()->getValue() || lnum->isNan()->getValue()) {
            return new NaN();
        }

        return new Number(lnum->getValue() / rnum->getValue());

    }

    /**
     * 12.6.3.3 Applying the % Operator
     * http://www.ecma-international.org/ecma-262/6.0/#sec-applying-the-mod-operator
     */
    static ESValue* modulo(ESValue* lref, ESValue* rref) {

        // lnum is the dividend
        Number* lnum = TypeOps::toNumber(lref);
        // rnum is the divisor
        Number* rnum = TypeOps::toNumber(rref);

        // If either operand is NaN, the result is NaN.
        if (lnum->isNan()->getValue() || lnum->isNan()->getValue()) {
            return new NaN();
        }

        // If the dividend is finite and the divisor is an infinity, the result equals the dividend.
        if (lnum->isFinite()->getValue() && rnum->isInfinity()->getValue()) {
            return lnum;
        }

        // If the dividend is a zero and the divisor is nonzero and finite, the result is the same as the dividend.
        if (lnum->getValue() == 0 && (rnum->getValue() != 0 && rnum->isFinite())) {
            return lnum;
        }

        // In the remaining cases, where neither an infinity, nor a zero, nor NaN is involved,
        // the floating-point remainder r from a dividend n and a divisor d is defined by the
        // mathematical relation r = n − (d × q) where q is an integer that is negative only
        // if n/d is negative and positive only if n/d is positive, and whose magnitude is as
        // large as possible without exceeding the magnitude of the true mathematical quotient
        // of n and d. r is computed and rounded to the nearest representable value using
        // IEEE 754-2008 round-to-nearest mode.
        double a = lnum->getValue();
        double b = rnum->getValue();
        return new Number(a - (b * (a / b)));

    }

    static ESValue* assign(ESValue* v, ESValue* w) {
        if (v->getType() == reference) {
            Reference* ref = dynamic_cast<Reference*>(v);

            if (ref != NULL) {
                return globalObj->set(ref->getReferencedName(), w);
            }

            throw TypeError;

        } else {
            throw ReferenceError;
        }
    }
    
   /* 12.10.3 Runtime Semantics: Evaluation EqualityExpression 
    * EqualityExpression == =! < > RelationalExpression - Only valid for number type
    */
    static bool evalee(ESValue* lhs, ESValue* rhs) {
    	 return TypeOps::toNumber(lhs) == TypeOps::toNumber(rhs);
    }
    
    static bool evalne(ESValue* lhs, ESValue* rhs) {
    	 return TypeOps::toNumber(lhs) != TypeOps::toNumber(rhs);
    }
    
    static bool evalgt(ESValue* lhs, ESValue* rhs) {
    	 return TypeOps::toNumber(lhs) > TypeOps::toNumber(rhs);
    }
    
    static bool evallt(ESValue* lhs, ESValue* rhs) {
    	 return TypeOps::toNumber(lhs) < TypeOps::toNumber(rhs);
    }
    
    /* 12.9.3 Runtime Semantics: Evaluation 
    * RelationalExpression : RelationalExpression <= ShiftExpression
    * RelationalExpression : RelationalExpression >= ShiftExpression
    */
    static bool evalltet(ESValue* lhs, ESValue* rhs) {
    	 return TypeOps::toNumber(lhs) <= TypeOps::toNumber(rhs);
    }
	
    static bool evalgtet(ESValue* lhs, ESValue* rhs) {
    	 return TypeOps::toNumber(lhs) <= TypeOps::toNumber(rhs);
    }

    /* 9.2.1 [[Call]] ( thisArgument, argumentsList)
     * The [[Call]] internal method for an ECMAScript function object F is called with parameters
     * thisArgument and argumentsList, a List of ECMAScript language values.
     */

    static ESValue* function(ESObject* thisArgument, ESValue* declarative, ESValue* param1, ESValue* param2) {
        Number* lnum = TypeOps::toNumber(param1);
        Number* rnum = TypeOps::toNumber(param2);
        Reference* ref;
        if (declarative->getType() == reference) {
            ref = dynamic_cast<Reference*>(declarative);
        }
        // If either operand is NaN, the result is NaN.
        if (lnum->isNan()->getValue() || rnum->isNan()->getValue()) {
            return new NaN();
        }
        globalObj->set(ref->getReferencedName(), thisArgument);
        return new Number(lnum->getValue() + rnum->getValue());
     }
};


