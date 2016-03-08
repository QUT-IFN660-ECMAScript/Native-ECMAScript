all:
	bison -d grammar.y && flex grammar.l && gcc lex.yy.c grammar.tab.c -o scanner -ll -ly