#pragma once

#include <map>
#include "../runtime/core.hpp"

enum TypeName {
    UNDEFINED,
    ES_NULL,
    BOOLEAN,
    STRING,
    SYMBOL,
    NUMBER,
    OBJECT
};

class ESValue {
public:
    virtual TypeName getType() = 0;
    virtual bool isPrimitive() = 0;
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

/**
 * For now Undefined just has a value of 0
 */
class Undefined : public Primitive<int> {
public:
    Undefined() {}

    TypeName getType() {
        return UNDEFINED;
    }

    int getValue() {
        return 0;
    }

    void setValue(int value) {
        return;
    }
};

/**
 * For now, Null also has a value of 0
 */
class Null : public Primitive<int> {
public:
    Null() {}

    TypeName getType() {
        return ES_NULL;
    }

    int getValue() {
        return 0;
    }

    void setValue(int value) {
        return;
    }
};

class Boolean : public Primitive<bool> {
private:
    bool value;
public:
    Boolean(bool value) {
        this->value = value;
    }

    TypeName getType() {
        return BOOLEAN;
    }

    bool getValue() {
        return value;
    }

    void setValue(bool value) {
        this->value = value;
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

    TypeName getType() {
        return STRING;
    }

    std::string getValue() {
        return value;
    }

    void setValue(std::string value) {
        this->value = value;
    }
};

class Symbol : public Primitive<std::string> {
private:
    std::string value;
public:
    Symbol(std::string value) {
        this->value = value;
    }

    TypeName getType() {
        return SYMBOL;
    }

    std::string getValue() {
        return value;
    }

    void setValue(std::string value) {
        this->value = value;
    }
};

class Number : public Primitive<double> {
private:
    double value;
public:
    Number(double value) {
        this->value = value;
    }

    TypeName getType() {
        return NUMBER;
    }

    double getValue() {
        return value;
    }

    void setValue(double value) {
        this->value = value;
    }
};

class Object : public ESValue {
public:
    TypeName getType() {
        return OBJECT;
    }

    bool isPrimitive() {
        return false;
    }
};

class Prototype : public Object {
private:
    std::map<std::string, ESValue*> prototype;
public:
    ESValue* get(ESValue* key_ref) {
        String* key = Core::toString(key);
        std::map<std::string, ESValue::*>::iterator it = prototype.find(key->getValue());
        if (it != prototype.end()) {
            return prototype[key->getValue()];
        }
    }

    void set(String key, ESValue* value) {
        prototype[key.getValue()] = value;
    }
};

class ESObject : public Object {
public:
    Prototype* prototype;
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
