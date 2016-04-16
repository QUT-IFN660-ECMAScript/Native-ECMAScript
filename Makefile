CC ?= $(shell command -v gcc)
CXX ?= $(shell command -v g++)
BABEL ?= $(shell command -v babel)

SHELL := $(shell echo $$SHELL)
.DEFAULT_GOAL := all

LEX_TESTS_DIR := tests/lex/test
LEX_ASSERTS_DIR := tests/lex/assert
LEX_TESTS := $(wildcard $(LEX_TESTS_DIR)/*.js)
TMP_DIR := tmp

all: clean .build_prod

clean: .clean_prod .clean_test
test_all: clean .setup_tests .run_babel_tests .run_lexer_test .teardown_tests
test_lexer: clean .setup_tests .run_lexer_test .teardown_tests
test_parser: clean .setup_tests .run_parser_test .teardown_tests
test: clean .setup_tests .run_lexer_test .run_parser_test .teardown_tests

.bison:
	@bison -d grammar.y
	$(info Parser Generated)
.flex:
	@flex --header-file=lex.yy.h grammar.l
	$(info Scanner Generated)

.clean_prod:
	@rm -f grammar.tab.* && rm -f lex.yy.* && rm -f scanner
.build_prod: .bison .flex
	@$(CC) lex.yy.c grammar.tab.c utils.c main.c -o scanner -ll -ly
	$(info Build Success)

.clean_test: .clean_prod
	@rm -f tests/test_lex
.build_lexer_test: .bison .flex
	@$(CC) lex.yy.c grammar.tab.c utils.c test_lex.c -o tests/test_lex -ll -ly
	$(info Build Lexer Success)
.build_parser_test: .bison .flex
	@$(CC) lex.yy.c grammar.tab.c utils.c test_parser.c -o tests/test_parser -ll -ly

.setup_tests:
	@rm -f log.txt;
	@touch log.txt;

.teardown_tests:
	$(if $(shell if [ -s "./log.txt" ]; then echo not empty; fi), $(error $(shell cat log.txt)), $(info All Tests Passed))

.run_babel_tests:
	$(foreach t, $(LEX_TESTS), \
	$(eval ASSERT_FILE=$(subst $(LEX_TESTS_DIR), $(LEX_ASSERTS_DIR), $(patsubst %.js, %.txt, $(t)))) \
	$(shell $(BABEL) $(t) >> /dev/null 2>>log.txt;))
	$(if $(shell if [ -s "./log.txt" ]; then echo not empty; fi), $(error $(shell cat log.txt)), $(info All JS files are valid))

# this seems a little crazy
.run_lexer_test: .build_lexer_test
	$(foreach t, $(LEX_TESTS), \
	$(eval ASSERT_FILE=$(subst $(LEX_TESTS_DIR), $(LEX_ASSERTS_DIR), $(patsubst %.js, %.txt, $(t)))) \
	$(shell diff $(ASSERT_FILE) <(./tests/test_lex < $(t))>> log.txt 2>&1;\
	))
	$(info Lexer Tests Finished)

.run_parser_test: .build_parser_test
	$(foreach t, $(LEX_TESTS), \
	$(shell ./tests/test_parser < $(t) >> log.txt 2>&1;\
	))
	$(info Parser Tests Finished)
