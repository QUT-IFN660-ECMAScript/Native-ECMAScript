CC ?= $(shell command -v gcc)
CXX ?= $(shell command -v g++)
BABEL ?= $(shell command -v babel)

SHELL := $(shell echo $$SHELL)
.DEFAULT_GOAL := all

TESTS_PATH := test
ASSERTS_PATH := assert
TESTS_ROOT := tests
TESTS := $(wildcard $(TESTS_ROOT)/**/test/*.js)
PARSABLE_TESTS_DIR := $(TESTS_ROOT)/parsable
PARSABLE_TESTS := $(wildcard $(PARSABLE_TESTS_DIR)/test/*.js)
TMP_DIR := tmp

ERROR_LOG := error.log
TEMP_ERROR_LOG := temperror.log

all: clean .build_prod

clean: .clean_prod
test_all: clean .setup_tests .run_babel_tests .run_lexer_test .run_parser_test .teardown_tests
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
	@rm -f grammar.tab.* && rm -f lex.yy.* && rm -f compiler
.build_prod: .bison .flex
	@$(CXX) -x c++ lex.yy.c grammar.tab.c utils.c main.cpp -o compiler -ll -ly
	$(info Build Success)

.build_lexer_test: .bison .flex
	@$(CC) lex.yy.c grammar.tab.c utils.c test_lex.c -o tests/test_lex -ll -ly
	$(info Build Lexer Success)
.build_parser_test: .bison .flex
	@$(CC) lex.yy.c grammar.tab.c utils.c test_parser.c -o tests/test_parser -ll -ly
	$(info Build Parser Success)

.setup_tests:
	@rm -f $(ERROR_LOG);
	@rm -f $(TEMP_ERROR_LOG);
	@touch $(ERROR_LOG);

.teardown_tests:
	$(if $(shell if [ -s "./$(ERROR_LOG)" ]; then echo not empty; fi), $(error $(shell cat $(ERROR_LOG))), $(info All Tests Passed))

.run_babel_tests:
	$(foreach t, $(TESTS), \
	$(eval ASSERT_FILE=$(subst /$(TESTS_PATH)/,/$(ASSERTS_PATH)/, $(patsubst %.js, %.txt, $(t)))) \
	$(shell $(BABEL) $(t) >> /dev/null 2>>$(ERROR_LOG);))
	$(if $(shell if [ -s "./$(ERROR_LOG)" ]; then echo not empty; fi), $(error $(shell cat $(ERROR_LOG))), $(info All JS files are valid))

# this seems a little crazy
.run_lexer_test: .build_lexer_test
	$(info Running Lexer Tests)
	$(foreach t, $(TESTS), \
	$(eval ASSERT_FILE=$(subst /$(TESTS_PATH)/,/$(ASSERTS_PATH)/, $(patsubst %.js, %.txt, $(t)))) \
	$(shell diff $(ASSERT_FILE) <(./tests/test_lex < $(t))>> $(TEMP_ERROR_LOG) 2>&1;\
	if [ -s "./$(TEMP_ERROR_LOG)" ]; then echo $(t) >> $(ERROR_LOG); cat $(TEMP_ERROR_LOG) >> $(ERROR_LOG); fi; rm -f $(TEMP_ERROR_LOG);\
	))

.run_parser_test: .build_parser_test
	$(info Running Parser Tests)
	$(foreach t, $(PARSABLE_TESTS), \
	$(shell touch $(TEMP_ERROR_LOG); ./tests/test_parser < $(t) >> $(TEMP_ERROR_LOG) 2>&1;\
	if [ -s "./$(TEMP_ERROR_LOG)" ]; then echo $(t) >> $(ERROR_LOG); cat $(TEMP_ERROR_LOG) >> $(ERROR_LOG); fi; rm -f $(TEMP_ERROR_LOG);\
	))
