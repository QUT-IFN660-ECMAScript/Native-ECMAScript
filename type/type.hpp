#pragma once
#include <map>
#include <sstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>


/**
 * This is really annoying, we need to append something like `_` because Type::string is c++11 and using just `string`
 * causes ambiguity errors with std::string
 */
enum Type {
    undefined,
    null,
    boolean,
    string_,
    symbol,
    number,
    object,
    reference
};

enum NumberType {
    posInfinity,
    negInfinity,
    notANumber
};

class String;

class ESValue {
public:
    virtual Type getType() = 0;
    virtual bool isPrimitive() = 0;
    /**
     * 7.1.12 ToString ( argument )
     * The abstract operation ToString converts argument to a value of type String
     */
    virtual String* toString() = 0;
};

template <class T>
class Primitive : public ESValue {
public:
    virtual T getValue() = 0;
    virtual void setValue(T value) = 0;

    bool isPrimitive() {
        return true;
    }
};

class String : public Primitive<std::string> {
private:
    std::string value;
public:
    String(std::string value) {
        this->value = value;
    }

    String() {
        this->value = std::string();
    }

    Type getType() {
        return string_;
    }

    std::string getValue() {
        return value;
    }

    void setValue(std::string value) {
        this->value = value;
    }

    String* toString() {
        return new String(value);
    }

    Primitive<std::string>* toPrimitive() {
        return this;
    }

};

/**
 * For now Undefined just has a value of 0
 */
class Undefined : public Primitive<Type > {
public:
    Undefined() {}

    Type getType() {
        return undefined;
    }

    Type getValue() {
        return undefined;
    }

    void setValue(Type value) {
        return;
    }

    String* toString() {
        return new String("undefined");
    }
};

/**
 * For now, Null also has a value of 0
 */
class Null : public Primitive<Type> {
public:
    Null() {}

    Type getType() {
        return null;
    }

    Type getValue() {
        return null;
    }

    void setValue(Type value) {
        return;
    }

    String* toString() {
        return new String("null");
    }
};

class Boolean : public Primitive<bool> {
private:
    bool value;
public:
    Boolean(bool value) {
        this->value = value;
    }

    Type getType() {
        return boolean;
    }

    bool getValue() {
        return value;
    }

    void setValue(bool value) {
        this->value = value;
    }

    String* toString() {
        return new String("null");
    }

};

class Symbol : public Primitive<std::string> {
private:
    std::string value;
public:
    Symbol(std::string value) {
        this->value = value;
    }

    Type getType() {
        return symbol;
    }

    std::string getValue() {
        return value;
    }

    void setValue(std::string value) {
        this->value = value;
    }

    String* toString() {
        return new String(this->value);
    }
};

/**
 * http://www.ecma-international.org/ecma-262/6.0/#sec-properties-of-the-number-constructor
 * TODO: implement the methods
 */
class Number : public Primitive<double> {
private:
    double value;
public:
    Number(){
        value = 0;
    }

    Number(double value) {
        this->value = value;
    }

    Type getType() {
        return number;
    }

    double getValue() {
        return value;
    }

    void setValue(double value) {
        this->value = value;
    }

    String* toString() {
        std::ostringstream strs;
        strs << value;
        return new String(strs.str());
    }

    Boolean* isNan() {
        return new Boolean(false);
    }

    Boolean* isFinite() {
        return new Boolean(true);
    }

    // isInfinity is a non-standard method, but I want it
    // in the ops for the runtime - harry
    Boolean* isInfinity() {
        return new Boolean(false);
    }

};

class NaN : public Number {
    Boolean* isNan() {
        return new Boolean(true);
    }

    Boolean* isFinite() {
        return new Boolean(false);
    }

    Boolean* isInfinity() {
        return new Boolean(false);
    }
};

class PosInfinity : public Number {
    Boolean* isNan() {
        return new Boolean(true);
    }

    Boolean* isFinite() {
        return new Boolean(false);
    }

    Boolean* isInfinity() {
        return new Boolean(true);
    }
};
class NegInfinity : public Number {
    Boolean* isNan() {
        return new Boolean(true);
    }

    Boolean* isFinite() {
        return new Boolean(false);
    }

    Boolean* isInfinity() {
        return new Boolean(true);
    }
};

class Object : public ESValue {
public:
    Type getType() {
        return object;
    }

    bool isPrimitive() {
        return false;
    }

};

class ESObject : public Object {
private:
    std::map<std::string, ESValue*> properties;
    ESObject* prototype;

public:
    ESObject() {
        properties.clear();
    }

    ESObject(ESObject* prototype) {
        this->prototype = prototype;
    }

    ESValue* get(ESValue* key_ref) {
        String* key = key_ref->toString();
        std::map<std::string, ESValue*>::iterator it = properties.find(key->getValue());
        if (it != properties.end()) {
            return properties[key->getValue()];
        }
        fprintf(stderr, "ya blew it!\n");
        return new Undefined();
    }

    ESValue* set(ESValue* key_ref, ESValue* value) {
        String* key = key_ref->toString();
        properties[key->getValue()] = value;
        return value;
    }


    String* toString() {
        return new String();
    }
};

class StringObject : public Object {
private:
    String* string;
public:
    StringObject() {
        string = new String();
    }

    StringObject(String* string) {
        this->string = string;
    }
};

class Function : public ESObject {

};

/**
 * 7.1 Type Conversion
 * http://www.ecma-international.org/ecma-262/6.0/#sec-toprimitive
 * The ECMAScript language implicitly performs automatic type conversion as needed. To clarify the semantics of certain
 * constructs it is useful to define a set of conversion abstract operations. The conversion abstract operations are
 * polymorphic; they can accept a value of any ECMAScript language type or of a Completion Record value. But no other
 * specification types are used with these operations.
 */
class TypeOps {
public:

    /**
     * 7.1.1 ToPrimitive ( input [, PreferredType] )
     * http://www.ecma-international.org/ecma-262/6.0/#sec-toprimitive
     * The abstract operation ToPrimitive takes an input argument and an optional argument PreferredType.
     * The abstract operation ToPrimitive converts its input argument to a non-Object type. If an object is capable of
     * converting to more than one primitive type, it may use the optional hint PreferredType to favour that type.
     * TODO: add the optional preferred type hint overload
     */
    static ESValue* toPrimitive(ESValue* input) {
        if (input->isPrimitive()) {
            Undefined* undefinedVal = dynamic_cast<Undefined*>(input);
            if (undefinedVal != NULL) {
                return undefinedVal;
            }

            Null* nullVal = dynamic_cast<Null*>(input);
            if (nullVal != NULL) {
                return nullVal;
            }

            String* stringVal = dynamic_cast<String*>(input);
            if (stringVal != NULL) {
                return stringVal;
            }

            Number* numberVal = dynamic_cast<Number*>(input);
            if (numberVal != NULL) {
                return numberVal;
            }

            Boolean* booleanVal = dynamic_cast<Boolean*>(input);
            if (booleanVal != NULL) {
                return booleanVal;
            }
        } else if (input->getType() == object) {
            //TODO: implement this correctly
            return new String("object[Object]");
        }
        return new Undefined();
    }


    /**
     * 7.1.2 ToBoolean ( argument )
     * http://www.ecma-international.org/ecma-262/6.0/#sec-toboolean
     * The abstract operation ToBoolean converts argument to a value of type Boolean.
     */
    static Boolean toBoolean(ESValue* argument) {
        switch (argument->getType()) {
            case undefined:
                return false;
            case null:
                return false;
            case boolean:
                return argument;
            /* 	Return false if argument is +0, −0, or NaN; otherwise return true. */
            case number:
                if (dynamic_cast<Number*>(argument)->getValue()== 0 ||
                	isnan(dynamic_cast<Number*>(argument)->getValue())) {
                	return false;
                }
                return true;
             /* Return false if argument is the empty String (its length is zero); otherwise return true */
            case string_:
            	return dynamic_cast<String*>(argument)==NULL;
            case symbol:
                return true;
            case object:
                return true;
            case reference:
                return false;
        }
    }

    /**
     * 7.1.3 ToNumber ( argument )
     * http://www.ecma-international.org/ecma-262/6.0/#sec-tonumber
     * The abstract operation ToNumber converts argument to a value of type Number
     */
    static Number* toNumber(ESValue* argument) {
        switch (argument->getType()) {
            case undefined:
                return new NaN();
            case null:
                return new Number(0);
            case boolean:
                if (dynamic_cast<Boolean*>(argument)->getValue()) {
                    return new Number(1);
                }
                return new Number(0);
            case number:
                return dynamic_cast<Number*>(argument);
          /*  The conversion of a String to a Number value is similar overall to the determination of the Number value 				for a numeric literal (see 11.8.3), but some of the details are different, process defined 7.1.3.11 . This 				value is determined in two steps: first, a mathematical value (MV) is derived from the String numeric 				literal; second, this mathematical value is rounded. */
            case string_:
				return mathematicalValue(argument);
            case symbol:
                // TODO: Throw a TypeError exception.
                return new NaN();
            case object:
                return toNumber(toPrimitive(argument));
            case reference:
                return NULL;
        }
    }
    
	/**
     * 7.1.4 ToInteger ( argument )
     * http://www.ecma-international.org/ecma-262/6.0/#sec-tointeger
     * The abstract operation ToInteger converts argument to an integral numeric value. 
     */
	static Number* toInteger(ESValue* argument) {
		Number* number = toNumber(argument);
		if (isnan(dynamic_cast<Number*>(number)->getValue())) {
			return new Number(0);
		} else {
			return new Number(floor(abs(dynamic_cast<Number*>(argument)->getValue())));
		}
	}
	
	
	/**
     * 7.1.12 ToString ( argument )
     * http://www.ecma-international.org/ecma-262/6.0/#sec-tostring
     * The abstract operation ToString converts argument to a value of type String 
     */
	static const char* toString(ESValue* argument) {
		switch (argument->getType()) {
            case undefined:
                return (const char*)"undefined";
            case null:
                return (const char*)"null";
            case boolean:
                if (dynamic_cast<Boolean*>(argument)->getValue()) {
                	return (const char*)"true";
                }
                return (const char*)"false";               
            case number:
            	return toStringOnNumber(argument);
            case symbol:
            	//todo
            	return (const char*)"true";
            case object:
            	//todo
            	return (const char*)"true";            		
		}
	}
	
	    
    /* Simple mathmatical evaluation - 7.1.3.1.1 Runtime Semantics: MV’s more complex for types 
       Return 0 if the length of string = 0 or string is completely whitespace otherwise return
       new Number
    */
    static Number* mathematicalValue(ESValue* argument) {
		if (dynamic_cast<String*>(argument)->getValue().length()==0) {
			return new Number(0);
		} else if (dynamic_cast<String*>(argument)->getValue().find_first_not_of(' ') != std::string::npos) {
			return new Number(0);
		} 
		return new Number(atof(dynamic_cast<String*>(argument)->getValue().c_str()));
	}
		
	
	/* 7.1.12.1 ToString Applied to the Number Type - Helper method for ToString(argument) */
	static const char* toStringOnNumber(ESValue* argument) {
		if (isnan(dynamic_cast<Number*>(argument)->getValue())) {
			return (const char*)"NaN";
		} else if (dynamic_cast<Number*>(argument)->getValue()==0) {
			return (const char*)"0";
		} else if isinf(dynamic_cast<Number*>(argument)->getValue()) {
			return (const char*)"Infinity";
		}
		return "0";
	}
		


};