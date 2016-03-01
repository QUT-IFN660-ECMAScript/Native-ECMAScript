all:
	flex grammar.l && gcc lex.yy.c -o lex