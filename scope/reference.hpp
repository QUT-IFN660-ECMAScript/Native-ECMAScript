//
// Created by Harry Scells on 3/05/2016.
//
#pragma once

#include <string>
#include "../type/type.hpp"

/**
 * 6.2.3 The Reference Specification Type
 * http://www.ecma-international.org/ecma-262/6.0/#sec-reference-specification-type
 *
 * A Reference is a resolved name or property binding. A Reference consists of three components, the base value, the
 * referenced name and the Boolean valued strict reference flag. The base value is either undefined, an Object, a
 * Boolean, a String, a Symbol, a Number, or an Environment Record (8.1.1). A base value of undefined indicates that
 * the Reference could not be resolved to a binding. The referenced name is a String or Symbol value.
 */
 
 /* Appendum: Refer http://www.ecma-international.org/ecma-262/6.0/#sec-getvalue
  * Important in runtime semantic of GetValue(V)
  */
  
class Reference{
public:
    virtual ESValue* getBase() = 0;
    virtual std::string getReferencedName() = 0;
    virtual bool isStrictReference() = 0;
    virtual bool hasPrimitiveBase() = 0;
    virtual bool isPropertyReference() = 0;
    virtual bool isUnresolvableReference() = 0;
    virtual bool isSuperReference() = 0;
};