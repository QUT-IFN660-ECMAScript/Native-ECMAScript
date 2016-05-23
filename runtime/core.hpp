//
// Created by Harry Scells on 8/05/2016.
//
#pragma once

#include "../type/type.hpp"
#include "../scope/reference.hpp"

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
     * TODO: comply with the spec, maybe even rename the variable names?
     */
    static ESValue* plus(ESValue* x_in, ESValue* y_in) {
        if (x_in->getType() == number && y_in->getType() == number) {

            Number* x = dynamic_cast<Number *> (x_in);
            Number* y = dynamic_cast<Number *> (y_in);

            return new Number(x->getValue() + y->getValue());
        } else {
            return new Undefined();
        }
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


