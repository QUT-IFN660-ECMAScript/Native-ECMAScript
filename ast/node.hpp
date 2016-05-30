#pragma once
#include <cstdarg>
#include <cstdio>
#include <iostream>


#include "../scope/lexical_scope.hpp"
//
// Created by Harry Scells on 18/04/2016.
//

extern int codeScopeDepth;
extern std::map<int, std::vector<std::string> > codeScope;

using namespace std;

class Node {
public:
	//This is used to generate the variable names of register in the pseudo machine code
	static int registerIndex;

	virtual void dump(int indent)=0;
	virtual unsigned int genCode() = 0;

	void emit(char* fmt, ...) {
		// previously we wrote directly to an output file here.
		char tempString[512] = {'\0'};
		va_list args;
		va_start(args, fmt);
//		vfprintf(outputfile, fmt, args);
//		fprintf(outputfile, "\n");
		vsnprintf(tempString, 512, fmt, args);
		codeScope[codeScopeDepth].push_back(tempString);
		va_end(args);
	}

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