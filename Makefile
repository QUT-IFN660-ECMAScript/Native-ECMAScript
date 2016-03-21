.DEFAULT_GOAL := all

.bison:
	@bison -d grammar.y
.flex:
	@flex grammar.l
.build_all:
	@gcc lex.yy.c grammar.tab.c main.c -o scanner -ll -ly
all: clean .bison .flex .build_all
clean:
	@rm -f grammar.tab.* && rm -f lex.yy.c && rm -f scanner