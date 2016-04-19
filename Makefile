CC ?= $(shell command -v gcc)
CXX ?= $(shell command -v g++)
BABEL ?= $(shell command -v babel)

CXX_FLAGS := -x c++ -Wno-write-strings

SHELL := $(shell echo $$SHELL)
.DEFAULT_GOAL := all

TESTS_PATH := test
LEXER_ASSERTS_PATH := lexer-assert

TESTS_ROOT := tests
TESTS := $(wildcard $(TESTS_ROOT)/**/$(TESTS_PATH)/*.js)

ERROR_LOG := error.log
TEMP_ERROR_LOG := temperror.log
PARSER_ERROR_LOG := error_parser.log

.checkdep:
ifndef CC
	$(error gcc not installed)
endif
ifndef CXX
	$(error g++ not installed)
endif
ifndef BABEL
	$(warning babel not installed, test_all will not run)
endif
.checkbabeldep:
ifndef BABEL
	$(error)
endif

all: .checkdep clean .build_prod
clean: .clean_prod
test_all: .checkdep .checkbabeldep clean .setup_tests .run_babel_tests .run_lexer_tests .run_parser_tests .teardown_tests
test_lexer: .checkdep clean .setup_tests .run_lexer_tests .teardown_tests
test_parser: .checkdep clean .setup_tests .run_parser_tests .teardown_tests
simple: .checkdep clean .run_simple
test: .checkdep clean .setup_tests .run_lexer_tests .run_parser_tests .teardown_tests
generate: .bison .flex

.bison:
	@bison -d grammar.y
	$(info Parser Generated)
.flex:
	@flex --header-file=lex.yy.h grammar.l
	$(info Scanner Generated)

.clean_prod:
	@rm -f grammar.tab.* && rm -f lex.yy.* && rm -f compiler
.build_prod: .bison .flex
	@$(CXX) $(CXX_FLAGS) lex.yy.c grammar.tab.c utils.c main.cpp -o compiler -ll -ly
	$(info Build Success)

.build_lexer_test: .bison .flex
	@$(CXX) $(CXX_FLAGS) lex.yy.c grammar.tab.c utils.c test_lex.c -o tests/test_lex -ll -ly
	$(info Build Lexer Success)
.build_parser_test: .bison .flex
	@$(CXX) $(CXX_FLAGS) lex.yy.c grammar.tab.c utils.c test_parser.cpp -o tests/test_parser -ll -ly
	$(info Build Parser Success)

# remove any previous ERROR_LOG and TEMP_ERROR_LOG, create new ERROR_LOG
.setup_tests:
	@rm -f $(ERROR_LOG);
	@rm -f $(TEMP_ERROR_LOG);
	@touch $(ERROR_LOG);

# if ERROR_LOG is not empty, throw make error,
# otherwise if PARSER_ERROR_LOG is not empty, show info message
# otherwise indicate all tests are passed
.teardown_tests:
	$(if $(shell if [ -s "./$(ERROR_LOG)" ]; then echo not empty; fi), \
		$(error $(shell cat $(ERROR_LOG))), \
		$(if $(shell if [ -s "./$(PARSER_ERROR_LOG)" ]; then echo not empty; fi), \
			$(info Ignored some parser errors, see $(PARSER_ERROR_LOG) for details),\
			$(info All Tests Passed)\
		)\
	)

# run babel transpiler on js files, if js file is invalid, it writes all stderr to ERROR_LOG
.run_babel_tests:
	$(foreach t, $(TESTS), \
		$(shell $(BABEL) $(t) >> /dev/null 2>>$(ERROR_LOG);))
		$(if $(shell if [ -s "./$(ERROR_LOG)" ]; then echo not empty; fi),\
		 	$(error $(shell cat $(ERROR_LOG))),\
		  $(info All JS files are valid))

# run lexer tests, use diff to compare lexer dump to expected output
.run_lexer_tests: .build_lexer_test
	$(info Running Lexer Tests)
	$(foreach t, $(TESTS), \
		$(eval ASSERT_FILE=$(subst /$(TESTS_PATH)/,/$(LEXER_ASSERTS_PATH)/, $(patsubst %.js, %.txt, $(t)))) \
		$(shell diff $(ASSERT_FILE) <(./tests/test_lex < $(t))>> $(TEMP_ERROR_LOG) 2>&1;\
			if [ -s "./$(TEMP_ERROR_LOG)" ]; then echo $(t) >> $(ERROR_LOG); \
				cat $(TEMP_ERROR_LOG) >> $(ERROR_LOG); fi; \
				rm -f $(TEMP_ERROR_LOG);\
		)\
	)

# test parseable tests, log any error to ERROR_LOG
# test unparseable tests, if tests are parseable, show warning to indicate tests are parseable
.run_parser_tests: .build_parser_test
	$(info Running Parser Tests)
	$(foreach t, $(wildcard ./$(TESTS_ROOT)/parseable/$(TESTS_PATH)/*.js), \
		$(shell touch $(TEMP_ERROR_LOG); ./tests/test_parser $(t) >> $(TEMP_ERROR_LOG) 2>&1;\
		if [ -s "./$(TEMP_ERROR_LOG)" ]; then echo $(t) >> $(ERROR_LOG); \
			cat $(TEMP_ERROR_LOG) >> $(ERROR_LOG); fi; rm -f $(TEMP_ERROR_LOG);\
		)\
	)
	$(foreach t, $(wildcard ./$(TESTS_ROOT)/unparseable/$(TESTS_PATH)/*.js), \
		$(if $(shell touch $(TEMP_ERROR_LOG); ./tests/test_parser $(t) >> $(TEMP_ERROR_LOG) 2>&1;\
			if [ -s "./$(TEMP_ERROR_LOG)" ]; then echo not empty; fi;\
				echo $(t) >> $(PARSER_ERROR_LOG); \
				cat $(TEMP_ERROR_LOG) >> $(PARSER_ERROR_LOG);\
				rm -f $(TEMP_ERROR_LOG);\
			),,\
			 $(warning $(t) is parseable, consider moving it to /parseable/)\
		)\
	)

.run_simple: .build_prod
	$(info Running Simple Test)
	@./compiler ./$(TESTS_ROOT)/parseable/$(TESTS_PATH)/string.js
	@./compiler ./$(TESTS_ROOT)/parseable/$(TESTS_PATH)/simple.js
