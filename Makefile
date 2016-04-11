SHELL := $(shell echo $$SHELL)
.DEFAULT_GOAL := all

CXXDEBUG = -g -Wall
CXXSTD = -std=c++11
CXXFLAGS = -Wno-deprecated-register -O0  $(CXXDEBUG) $(CXXSTD)

LEX_TESTS_DIR := tests/lex/test
LEX_ASSERTS_DIR := tests/lex/assert
LEX_TESTS := $(wildcard $(LEX_TESTS_DIR)/*.js)
TMP_DIR := tmp

.bison:
	@bison parser.yy
	$(info Parser Generated)
.flex:
	@flex lexer.l
	$(info Scanner Generated)

.clean_prod:
	@rm -f parser.tab.* && rm -f lex.yy.* && rm -f scanner && rm -f position.hh stack.hh location.hh
.build_prod: .bison .flex
	@g++ $(CXXFLAGS) lex.yy.cc parser.tab.cc driver.cpp main.cpp -o scanner
	$(info Build Success)

.clean_test: .clean_prod
	@rm -f tests/test_lex
.build_lex_test: .bison .flex
	@g++ $(CXXFLAGS) lex.yy.cc parser.tab.cc driver.cpp test_lex.cpp -o tests/test_lex
	$(info Build Success)



all: clean .build_prod
clean: .clean_prod .clean_test
test: clean .run_lex_test
flex: .flex
bison: .bison

# this seems a little crazy
.run_lex_test: .build_lex_test
	@touch log.txt;
	$(foreach t, $(LEX_TESTS), \
	$(shell \
	 	diff $(subst $(LEX_TESTS_DIR), $(LEX_ASSERTS_DIR), $(patsubst %.js, %.txt, $(t))) \
		<(./tests/test_lex < $(t)) >> log.txt 2>&1;\
	))
	$(if $(shell if [ -s "./log.txt" ]; then echo not empty; fi), $(error $(shell cat log.txt; rm -f log.txt)), $(info All Tests Passed))
	@rm -f log.txt
