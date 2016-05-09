//
// Created by Harry Scells on 8/05/2016.
//
#pragma once

#include <sstream>
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

            return new Number(x->getValue() + y->getValue());
        } else {
            return new Undefined();
        }
    }

    /**
     * 7.1.12 ToString ( argument )
     * The abstract operation ToString converts argument to a value of type String
     */
    static String* toString(ESValue* argument) {
        if (argument->getType() == UNDEFINED) {
            return new String("undefined");
        } else if (argument->getType() == NULL) {
            return new String("null");
        } else if (argument->getType() == BOOLEAN) {
            Boolean* boolean = dynamic_cast<Boolean*>(argument);
            if (boolean != NULL) {
                if (boolean->getValue()) {
                    return new String("true");
                }
                return new String("false");
            }
            return new Undefined();
        } else if (argument->getType() == NUMBER) { // 7.1.12.1 ToString Applied to the Number Type
            // TODO: Implement this better!
            Number* number = dynamic_cast<Number*>(argument);
            if (number != NULL) {
                std::osstringstream strs;
                strs << number->getValue();
                return new String(strs.str());
            }
        }
    }
};

