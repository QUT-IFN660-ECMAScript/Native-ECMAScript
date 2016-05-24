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
class Reference : public ESValue {
private:
    ESValue* base;
    String* referencedName;
    Boolean* strict;
public:

    Reference(String* referencedName) {
     this->referencedName = referencedName;
     this->base = new Undefined();
     this->strict = new Boolean(false);
    }

    Reference(String* referencedNames, ESValue* base) {
     this->referencedName = referencedNames;
     this->base = base;
     this->strict = new Boolean(false);
    }

    Type getType() {
     return reference;
    }

    bool isPrimitive() {
     return false;
    }

    String* toString() {
     return referencedName;
    }

	// 6.2.3 The Reference Specification Type
    // Implementations of the following abstract operations:   
    // -----------------------------------------------------

    // The following abstract operations are used in this specification to access the components of references:

    // GetBase(V). Returns the base value component of the reference V.
    ESValue* getBase() {
     return base;
    }

    // GetReferencedName(V). Returns the referenced name component of the reference V.
    String* getReferencedName() {
     return referencedName;
    }

    // IsStrictReference(V). Returns the strict reference flag component of the reference V.
    Boolean* isStrictReference() {
     return strict;
    }

    // HasPrimitiveBase(V). Returns true if Type(base) is Boolean, String, Symbol, or Number.
    Boolean* hasPrimitiveBase() {
     return new Boolean(base->isPrimitive());
    }


    // TODO:
    // IsSuperReference(V). Returns true if this reference has a thisValue component.
    
    /* Returns true if either the base value is an object or HasPrimitiveBase(V) is true; otherwise returns false. */
    Boolean* IsPropertyReference() {
    	return new Boolean(((base->getType()==object) || hasPrimitiveBase()));
    }
    
    //Returns true if the base value is undefined and false otherwise.
    Boolean IsUnresolvableReference() {
    	return new Boolean(base->getType()==undefined);
    }
   

};