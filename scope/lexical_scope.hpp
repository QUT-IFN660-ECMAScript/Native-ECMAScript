//
// Created by Harry Scells on 3/05/2016.
//
#pragma once

#include <map>
#include "declaration.hpp"

class LexicalScope {
protected:
    LexicalScope* parentScope;
    std::map<std::string, Declaration*> symbolTable;

public:
    LexicalScope() {
        // printf("test\n");
        parentScope = NULL;
        symbolTable.clear();
    }

    Declaration* resolveHere(std::string symbol) {
        std::map<std::string, Declaration*>::iterator it = symbolTable.find(symbol);
        if (it != symbolTable.end()) {
            return it->second;
        }
        return NULL;
    }

    Declaration* resolve(std::string symbol) {
        // printf("%s\n", symbol.c_str());
        Declaration* local = resolveHere(symbol);
        if (local != NULL) {
            return local;
        } else if (parentScope != NULL) {
            return parentScope->resolve(symbol);
        }
        return NULL;
    }

    void addToSymbolTable(std::string symbol, Declaration* declaration) {
        symbolTable[symbol] = declaration;
    }
};