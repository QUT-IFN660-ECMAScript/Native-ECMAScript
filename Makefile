bison:
	@bison -d grammar.y
flex:
	@flex grammar.l
build:
	@gcc lex.yy.c grammar.tab.c main.c -o scanner -ll -ly

all: bison flex build

clean:
	@rm -f grammar.tab.* && rm -f lex.yy.c && rm -f scanner

