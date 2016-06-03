//
// Created by Harry Scells on 8/05/2016.
//
#pragma once

#include "../type/type.hpp"
#include "../scope/reference.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <stdint.h>

enum Exception {
    ReferenceError,
    TypeError
};

extern ESObject* globalObj;

class Core {

public:
    /**
     * Helper method - In simple evluation the Global Object is always thisArgument
     * precondition: global object exists
     * postcondition: global object assigned to thisValue
     */
     static void assignGlobalObject() {
        static ESObject* thisArgument;
        ESObject* globalObj = new ESObject;
        globalObj = thisArgument;
     }

    /**
     * 12.7.3 The Addition operator ( + )
     * http://www.ecma-international.org/ecma-262/6.0/#sec-addition-operator-plus
     */
    static ESValue* plus(ESValue* lref, ESValue* rref) {

        Number* lnum = TypeOps::toNumber(lref);
        Number* rnum = TypeOps::toNumber(rref);
        assignGlobalObject();

        // If either operand is NaN, the result is NaN.
        if (validateNaN(lnum, rnum)) {
            return new NaN();
        }

        return new Number(lnum->getValue() + rnum->getValue());
    }

    /** 12.5.9.1 Runtime Semantics: Evaluation
     * The unary + operator converts its operand to Number type.
     * precondition: rref initialised
     * postcondition: plus operation applied
     */
    static ESValue* plus_u(ESValue* rref) {

        Number* rnum = TypeOps::toNumber(rref);
        static ESObject* thisArgument;
        assignGlobalObject();

        // If operand is NaN, the result is NaN.
        if (rnum->isNan()->getValue()) {
            return new NaN();
        }

        return new Number(rnum->getValue());
    }

     /** 12.5.10.1 Runtime Semantics: Evaluation
     * The unary - operator converts its operand to Number type and then negates it. 
     * Negating +0 produces −0, and negating −0 produces +0.
     * precondition: rref initialised
     * postcondition: subtract operation applied
     */
    static ESValue* subtract_u(ESValue* rref) {

        Number* rnum = TypeOps::toNumber(rref);
        assignGlobalObject();

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
        assignGlobalObject();

        // If either operand is NaN, the result is NaN.
       if (validateNaN(lnum, rnum)) {
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
        assignGlobalObject();

        // If either operand is NaN, the result is NaN.
        if (validateNaN(lnum, rnum)) {
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
        assignGlobalObject();

        // If either operand is NaN, the result is NaN.
        if (validateNaN(lnum, rnum)) {
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
        assignGlobalObject();

        // If either operand is NaN, the result is NaN.
        if (validateNaN(lnum, rnum)) {
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
            assignGlobalObject();

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

    /* Evaluate that both lhs and rhs are exactly equal
     * precondition: lhs & rhs initialised
     * postcondition: return true if both values equal else false
     */
    static bool evalee(ESValue* lhs, ESValue* rhs) {
        assignGlobalObject();
        Number* lnum = TypeOps::toNumber(lhs);
        Number* rnum = TypeOps::toNumber(rhs);
        return lnum == rnum;
    }
    
    /* Evaluate that both lhs and rhs are not equal
     * precondition: lhs & rhs initialised
     * postcondition: return true if both values not equal else false
     */
    static bool evalne(ESValue* lhs, ESValue* rhs) {
        assignGlobalObject();
        Number* lnum = TypeOps::toNumber(lhs);
        Number* rnum = TypeOps::toNumber(rhs);
        return lnum != rnum;
    }
    
    /* Evaluate that lhs > rhs
     * precondition: lhs & rhs initialised
     * postcondition: return true if lhs > rhs else false
     */
    static bool evalgt(ESValue* lhs, ESValue* rhs) {
        assignGlobalObject();
        Number* lnum = TypeOps::toNumber(lhs);
        Number* rnum = TypeOps::toNumber(rhs);
        return lnum > rnum;
    }
    
    /* Evaluate that lhs < rhs
     * precondition: lhs & rhs initialised
     * postcondition: return true if lhs < rhs else false
     */
    static bool evallt(ESValue* lhs, ESValue* rhs) {
        assignGlobalObject();
        Number* lnum = TypeOps::toNumber(lhs);
        Number* rnum = TypeOps::toNumber(rhs);
        return lnum < rnum;
    }
    
    /* 12.9.3 Runtime Semantics: Evaluation 
    * RelationalExpression : RelationalExpression <= ShiftExpression
    * RelationalExpression : RelationalExpression >= ShiftExpression
    */

    /* Evaluate that lhs <= rhs
     * precondition: lhs & rhs initialised
     * postcondition: return true if lhs <= rhs else false
     */
    static bool evalltet(ESValue* lhs, ESValue* rhs) {
        assignGlobalObject();
        Number* lnum = TypeOps::toNumber(lhs);
        Number* rnum = TypeOps::toNumber(rhs);
        return lnum <= rnum;
    }
	
    /* Evaluate that lhs >= rhs
     * precondition: lhs & rhs initialised
     * postcondition: return true if lhs >= rhs else false
     */
    static bool evalgtet(ESValue* lhs, ESValue* rhs) {
        assignGlobalObject();
        Number* lnum = TypeOps::toNumber(lhs);
        Number* rnum = TypeOps::toNumber(rhs);
        return lnum >= rnum;
    }

    /* Evaluate that lhs === rhs
     * precondition: lhs & rhs initialised
     * postcondition: return true if lhs == rhs (type/value) else false
     */
    static bool evaleee(ESValue* lhs, ESValue* rhs) {
        assignGlobalObject();
        Number* lnum = TypeOps::toNumber(lhs);
        Number* rnum = TypeOps::toNumber(rhs);
        return ((lnum == rnum) && (lnum->getType()==rnum->getType());
    }

    /* Evaluate that lhs >= rhs
     * precondition: lhs & rhs initialised
     * postcondition: return true if lhs != rhs (value/type) else false
     */
    static bool evalnee(ESValue* lhs, ESValue* rhs) {
        assignGlobalObject();
        Number* lnum = TypeOps::toNumber(lhs);
        Number* rnum = TypeOps::toNumber(rhs);
        return ((lnum != rnum) && (lnum->getType() != run->getType());
    }

    /* 9.2.1 [[Call]] ( thisArgument, argumentsList)
     * The [[Call]] internal method for an ECMAScript function object F is called with parameters
     * thisArgument and argumentsList, a List of ECMAScript language values.
     */

     /* Evaluate function operation
     * precondition: thisArgument, function declaration, parameter list (todo) initialised, function body initialised
     * postcondition: result of function body evaluated 
     */
    static ESValue* function(ESObject* thisArgument, ESValue* declarative, ESValue* param1, ESValue* param2) {
        Number* lnum = TypeOps::toNumber(param1);
        Number* rnum = TypeOps::toNumber(param2);
        Reference* ref;
        if (declarative->getType() == reference) {
            ref = dynamic_cast<Reference*>(declarative);
        }

        if (validateNaN(lnum,rnum)) {
            return new NaN();
        }

        globalObj->set(ref->getReferencedName(), thisArgument);
        return new Number(lnum->getValue() + rnum->getValue());
     }

     /* 12.8.3.1 Runtime Semantics: Evaluation
      * Evaluation method for the left shift operation lhs << rhs = x * 2^y
      * For number operations - return the result of bit shift operation
      * precondition: lhs & rhs initialised
      * postcondition: operator shifts the first operand the specified number of bits to the left
      */
    static ESValue* shiftl(ESValue* lhs, ESValue* rhs) {
        Number* lnum = TypeOps::toNumber(lhs);
        Number* rnum = TypeOps::toNumber(rhs);

        if (validateNaN(lnum,rnum)) {
            return new NaN();
        }
        return new Number((uint32_t)lnum->getValue() << (uint32_t)rnum->getValue()); 
    }

     /* 12.8.4.1 Runtime Semantics: Evaluation
      * Evaluation method for the right shift operation lhs >> rhs 
      * For number operations - return the result of bit shift operation
      * precondition: lhs & rhs initialised
      * postcondition: operator shifts the first operand the specified number of bits to the left
      */
    static ESValue* shiftr(ESValue* lhs, ESValue* rhs) {
        Number* lnum = TypeOps::toNumber(lhs);
        Number* rnum = TypeOps::toNumber(rhs);

        if (validateNaN(lnum,rnum)) {
            return new NaN();
        }
        return new Number((uint32_t)lnum->getValue() >> (uint32_t)rnum->getValue()); 
    }

    /* 12.8.5.1 Runtime Semantics: Evaluation
     * Evaluation method for the zero fill shift operation lhs >>> rhs 
      * For number operations - return the result of bit shift operation
      * precondition: lhs & rhs initialised
      * postcondition: operator shifts the first operand the specified number of bits to the right.
      */
    static ESValue* shiftzr(ESValue* lhs, ESValue* rhs) {
        Number* lnum = TypeOps::toNumber(lhs);
        Number* rnum = TypeOps::toNumber(rhs);

        if (validateNaN(lnum,rnum)) {
            return new NaN();
        }
        return new Number((uint32_t)lnum->getValue() >> (uint32_t)rnum->getValue()); 
    }

    /* Helper function - Evaluation of either value is NaN 
     * precondition: ref1 & ref2 initialised
     * postconditions: returns NaN if either value is Nan
     */
    static bool validateNaN(Number* ref1, Number* ref2) {
        return (ref1->isNan()->getValue() || ref2->isNan()->getValue());
    }
};


