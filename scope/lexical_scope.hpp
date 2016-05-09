//
// Created by Harry Scells on 3/05/2016.
//
#pragma once

#include <map>
#include "reference.hpp"

class LexicalScope {
protected:
    LexicalScope* parentScope;
    std::map<std::string, Reference*> symbolTable;

public:
    LexicalScope() {
        // printf("test\n");
        parentScope = NULL;
        symbolTable.clear();
    }

    Reference* resolveHere(std::string symbol) {
        std::map<std::string, Reference*>::iterator it = symbolTable.find(symbol);
        if (it != symbolTable.end()) {
            return it->second;
        }
        return NULL;
    }

    Reference* resolve(std::string symbol) {
        // printf("%s\n", symbol.c_str());
        Reference* local = resolveHere(symbol);
        if (local != NULL) {
            return local;
        } else if (parentScope != NULL) {
            return parentScope->resolve(symbol);
        }
        return NULL;
    }

    void addToSymbolTable(std::string symbol, Reference* reference) {
        symbolTable[symbol] = reference;
    }
};