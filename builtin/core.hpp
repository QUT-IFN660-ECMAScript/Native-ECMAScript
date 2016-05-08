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
    static ESValue* Plus(ESValue* x_in, ESValue* y_in) {
        if (x_in->getType() == NUMBER && y_in->getType() == NUMBER) {

            Number* x = dynamic_cast<Number *> (x_in);
            Number* y = dynamic_cast<Number *> (y_in);

            double x_val;
            double y_val;

            if (x->getValue().i_value) {
                x_val = (double) x->getValue().i_value;
            } else {
                x_val = x->getValue().d_value;
            }

            if (y->getValue().i_value) {
                y_val = (double) y->getValue().i_value;
            } else {
                y_val = y->getValue().d_value;
            }

            return new Number(x_val + y_val);
        } else {
            return new Undefined();
        }
    }
};

