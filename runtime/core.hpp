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

};


