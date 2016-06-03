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
    static bool zeroFlag;
    /**
     * 12.7.3 The Addition operator ( + )
     * http://www.ecma-international.org/ecma-262/6.0/#sec-addition-operator-plus
     */
    static ESValue* plus(ESValue* lref, ESValue* rref) {

        Number* lnum = TypeOps::toNumber(lref);
        Number* rnum = TypeOps::toNumber(rref);

        // If either operand is NaN, the result is NaN.
        if (lnum->isNan()->getValue() || lnum->isNan()->getValue()) {
            return new NaN();
        }

        return new Number(lnum->getValue() + rnum->getValue());
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

    /*
     * Strict Equality Comparison
     * http://www.ecma-international.org/ecma-262/6.0/#sec-strict-equality-comparison
     */
    static Boolean strictEqualityComparison(ESValue* left, ESValue* right) {
        Type leftType = left->getType();
        Type rightType = right->getType();

        if(leftType != rightType) {
            zeroFlag = false;
            return new Boolean(false);
        }
        switch (leftType) {
            case undefined: {
                zeroFlag = true;
                return new Boolean(true);
            }
            case null: {
                zeroFlag = true;
                return new Boolean(true);
            }
            case boolean: {
                if (rightType != boolean) {
                    zeroFlag = false;
                    return new Boolean(true);
                } else {
                    Boolean* leftBool = dynamic_cast<Boolean*>(left);
                    Boolean* rightBool = dynamic_cast<Boolean*>(right);                    
                    if (leftBool->getValue() && rightBool->getValue()
                    || !leftBool->getValue() && !rightBool->getValue()) {
                        zeroFlag = true;
                        return new Boolean(true);
                    }
                    zeroFlag = false;
                    return new Boolean(false);
                }
            }
            case string_: {
                if (rightType != string_) {
                    zeroFlag = false;
                    return new Boolean(false);
                }
                String* leftStr = dynamic_cast<String*>(left);
                String* rightStr = dynamic_cast<String*>(right);
                if(leftStr->getValue() == rightStr->getValue()) {
                    zeroFlag = true;
                    return new Boolean(true);
                } else {
                    zeroFlag = false;
                    return new Boolean(false);
                }
            }
                
            case symbol: {
                if (rightType != symbol) {
                    zeroFlag = false;
                    return new Boolean(false);
                } else {
                    Symbol* leftSymbol = dynamic_cast<Symbol*>(left);
                    Symbol* rightSymbol = dynamic_cast<Symbol*>(right);                    
                    if (leftSymbol->getValue() == rightSymbol->getValue()) {
                        zeroFlag = true;
                        return new Boolean(true);
                    }
                    zeroFlag = false;
                    return new Boolean(false);
                }
            }
            case number: {
                //TODO: Implement this... 
                // If x is +0 and y is −0, return true.
                // If x is −0 and y is +0, return true.
                Number* leftNum = dynamic_cast<Number*>(left);
                if(leftNum->isNan()) {
                    zeroFlag = false;
                    return new Boolean(false);
                }
                if (rightType != number) {
                    zeroFlag = false;
                    return new Boolean(false);
                }
                Number* rightNum = dynamic_cast<Number*>(right);
                if(rightNum->isNan()){
                    zeroFlag = false;
                    return new Boolean(false);
                }
                if (leftNum->getValue() == rightNum->getValue()) {
                    zeroFlag = true;
                    return new Boolean(true);
                }
                zeroFlag = false;
                return new Boolean(false);
            }
            case object: {
                if (rightType != object) {
                    zeroFlag = false;
                    return new Boolean(false);
                } else {
                    Symbol* leftObj = dynamic_cast<Symbol*>(left);
                    Symbol* rightObj = dynamic_cast<Symbol*>(right);                    
                    if (leftObj->getValue() == rightObj->getValue()) {
                        zeroFlag = true;
                        return new Boolean(true);
                    }
                    zeroFlag = false;
                    return new Boolean(false);
                }
            }

        }// end of switch (leftType) 
        
    }

};


