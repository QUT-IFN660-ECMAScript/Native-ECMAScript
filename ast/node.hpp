#pragma once
#include <cstdarg>
#include <cstdio>
#include <iostream>

#include "lexicalscope.hpp"
//
// Created by Harry Scells on 18/04/2016.
//
using namespace std;

class Node {
public:
	virtual void dump(int indent)=0;
	virtual bool resolveNames(LexicalScope* scope) = 0;
	void indent(int N) {
		for (int i = 0; i < N; i++)
			printf("    ");
	}
	void label(int i, char const* fmt, ...) {
		indent(i);
		va_list args;
		va_start(args, fmt);
		vprintf(fmt, args);
		va_end(args);
	}
	void dump(int i, char const* name) {
		label(i, "%s:\n", name);
		dump(i + 1);
	}
};
