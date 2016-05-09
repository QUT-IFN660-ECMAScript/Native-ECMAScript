//
// Created by Harry Scells on 8/05/2016.
//
#pragma once

#include <iostream>
#include "../type/type.hpp"

/**
 * This isn't standard ECMA, but most JS interpreters use it
 */
class Console {

public:

    /**
     * Arbitrarily log some value to the screen
     */
    static void log(ESValue* value) {
        if (value->isPrimitive()) {
            if (value->getType() == UNDEFINED) {
                fprintf(stdout, "undefined\n");
                return;
            } else if (value->getType() == ES_NULL) {
                fprintf(stdout, "null\n");
                return;
            } else if (value->getType() == BOOLEAN) {
                Boolean* boolean = dynamic_cast<Boolean*>(value);
                if (boolean != NULL) {
                    if (boolean->getValue()) {
                        fprintf(stdout, "true\n");
                    } else {
                        fprintf(stdout, "false\n");
                    }
                } else {
                    fprintf(stderr, "unloggable type (failed to cast to bool)\n");
                }
                return;
            } else if (value->getType() == STRING) {
                String* string = dynamic_cast<String*>(value);
                if (string != NULL) {
                    fprintf(stdout, "%s\n", string->getValue().c_str());
                } else {
                    fprintf(stderr, "unloggable type (failed to cast to string)\n");
                }
                return;
            } else if (value->getType() == SYMBOL) {
                Symbol *symbol = dynamic_cast<Symbol *>(value);
                if (symbol != NULL) {
                    fprintf(stdout, "%s\n", symbol->getValue().c_str());
                } else {
                    fprintf(stderr, "unloggable type (failed to cast to symbol)\n");
                }
                return;
            } else if (value->getType() == NUMBER) {
                Number *number = dynamic_cast<Number *>(value);
                if (number != NULL) {
                    fprintf(stdout, "%f\n", number->getValue());
                } else {
                    fprintf(stderr, "unloggable type (failed to cast to number)\n");
                }
                return;
            } else if (value->getType() == OBJECT) {
                fprintf(stdout, "Object[object]\n");
                return;
            }
        } else {
            fprintf(stderr, "unloggable type\n");
        }
    }

};