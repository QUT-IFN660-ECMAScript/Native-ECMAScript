all:
	flex --c++ grammar.l && g++ lex.yy.cc -o scanner