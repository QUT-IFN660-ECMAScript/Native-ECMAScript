.DEFAULT_GOAL := all

.bison:
	@bison -d grammar.y
.flex:
	@flex --header-file=lex.yy.h grammar.l
.build_all:
	@gcc lex.yy.c grammar.tab.c main.c -o scanner -ll -ly
all: clean .bison .flex .build_all
clean:
	@rm -f grammar.tab.* && rm -f lex.yy.* && rm -f scanner