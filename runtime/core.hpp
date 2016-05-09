//
// Created by Harry Scells on 8/05/2016.
//
#pragma once

#include "../type/type.hpp"

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

};

