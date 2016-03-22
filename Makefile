SHELL := $(shell echo $$SHELL)
.DEFAULT_GOAL := all


LEX_TESTS_DIR := tests/lex/test
LEX_ASSERTS_DIR := tests/lex/assert
LEX_TESTS := $(wildcard $(LEX_TESTS_DIR)/*.js)
TMP_DIR := tmp

.bison:
	@bison -d grammar.y
.flex:
	@flex --header-file=lex.yy.h grammar.l

.clean_prod:
	@rm -f grammar.tab.* && rm -f lex.yy.* && rm -f scanner
.build_prod: .bison .flex
	@gcc lex.yy.c grammar.tab.c utils.c main.c -o scanner -ll -ly

.clean_test: .clean_prod
	@rm -f tests/test_lex
.build_lex_test: .bison .flex
	@gcc lex.yy.c grammar.tab.c utils.c test_lex.c -o tests/test_lex -ll -ly

# this seems a little crazy
.run_lex_test: .build_lex_test
	$(foreach t, $(LEX_TESTS), \
	$(shell \
		diff $(subst $(LEX_TESTS_DIR), $(LEX_ASSERTS_DIR), $(patsubst %.js, %.txt, $(t))) \
		<(./tests/test_lex < $(t)); \
	))

all: clean .build_prod
clean: .clean_prod .clean_test
test: clean .run_lex_test
