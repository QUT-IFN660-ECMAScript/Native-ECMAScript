#pragma once

#include <map>
#include <sstream>

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
    object
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
};

/**
 * For now Undefined just has a value of 0
 */
class Undefined : public Primitive<int> {
public:
    Undefined() {}

    Type getType() {
        return undefined;
    }

    int getValue() {
        return 0;
    }

    void setValue(int value) {
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

class Number : public Primitive<double> {
private:
    double value;
public:
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

class Prototype : public Object {
private:
    std::map<std::string, ESValue*> prototype;
public:
    Prototype() {
        prototype.clear();
    }

    ESValue* get(ESValue* key_ref) {
        String* key = key_ref->toString();
        std::map<std::string, ESValue*>::iterator it = prototype.find(key->getValue());
        if (it != prototype.end()) {
            return prototype[key->getValue()];
        }
        fprintf(stderr, "ya blew it!\n");
        return new Undefined();
    }

    void set(ESValue* key_ref, ESValue* value) {
        String* key = key_ref->toString();
        prototype[key->getValue()] = value;
    }

    String* toString() {
        return new String();
    }
};

class ESObject : public Object {
private:
    std::map<std::string, ESValue*> properties;
public:
    Prototype* prototype;

    ESObject() {
        this->prototype = new Prototype();
        properties.clear();
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

    void set(ESValue* key_ref, ESValue* value) {
        String* key = key_ref->toString();
        properties[key->getValue()] = value;
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
