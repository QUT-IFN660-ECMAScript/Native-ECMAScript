#pragma once

#include <map>
#include <sstream>


enum TypeName {
    UNDEFINED,
    ES_NULL,
    BOOLEAN,
    STRING,
    SYMBOL,
    NUMBER,
    OBJECT
};

class String;

class ESValue {
public:
    virtual TypeName getType() = 0;
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

    TypeName getType() {
        return STRING;
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

    TypeName getType() {
        return UNDEFINED;
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

    TypeName getType() {
        return BOOLEAN;
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

    TypeName getType() {
        return SYMBOL;
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

    TypeName getType() {
        return NUMBER;
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
    TypeName getType() {
        return OBJECT;
    }

    bool isPrimitive() {
        return false;
    }

};

class Prototype : public Object {
private:
    std::map<String*, ESValue*> prototype;
public:
    ESValue* get(ESValue* key_ref) {
        String* key = key_ref->toString();
        std::map<String*, ESValue*>::iterator it = prototype.find(key);
        if (it != prototype.end()) {
            return prototype[key];
        }
        fprintf(stderr, "ya blew it!\n");
        return new Null();
    }

    void set(String* key, ESValue* value) {
        prototype[key] = value;
    }

    String* toString() {
        return new String();
    }
};

class ESObject : public Object {
public:
    Prototype* prototype;

    ESObject() {
        this->prototype = new Prototype();
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
