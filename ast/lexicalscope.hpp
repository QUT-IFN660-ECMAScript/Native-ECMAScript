#pragma once
#include <map>
#include "declaration.hpp"

using namespace std;
class LexicalScope {
protected:
	LexicalScope* parentScope;
	map<string, Declaration*> symbol_table;

public:
	Declaration* resolveHere(string symbol) {
		map<string, Declaration*>::iterator iter = symbol_table.find(symbol);
		if(iter != symbol_table.end()) {
			return iter->second;
		}

		return NULL;
	}

	Declaration* resolve (string symbol) {
		Declaration* declaration = resolveHere (symbol);
		if(declaration != NULL) {
			return declaration;
		} else if (parentScope != NULL) {
			parentScope->resolve(symbol);
		} else {
			return NULL;
		}
	}
};