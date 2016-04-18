CC ?= $(shell command -v gcc)
CXX ?= $(shell command -v g++)
BABEL ?= $(shell command -v babel)

SHELL := $(shell echo $$SHELL)
.DEFAULT_GOAL := all

TESTS_PATH := test
ASSERTS_PATH := assert

TESTS_ROOT := tests
TESTS := $(wildcard $(TESTS_ROOT)/**/$(TESTS_PATH)/*.js)

ERROR_LOG := error.log
TEMP_ERROR_LOG := temperror.log
PARSER_ERROR_LOG := error_parser.log

all: clean .build_prod

clean: .clean_prod
test_all: clean .setup_tests .run_babel_tests .run_lexer_test .run_parser_test .teardown_tests
test_lexer: clean .setup_tests .run_lexer_tests .teardown_tests
test_parser: clean .setup_tests .run_parser_tests .teardown_tests
test: clean .setup_tests .run_lexer_tests .run_parser_tests .teardown_tests

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
	# remove any previous ERROR_LOG and TEMP_ERROR_LOG, create new ERROR_LOG
	@rm -f $(ERROR_LOG);
	@rm -f $(TEMP_ERROR_LOG);
	@touch $(ERROR_LOG);

.teardown_tests:
	# if ERROR_LOG is not empty, throw make error,
	# otherwise if PARSER_ERROR_LOG is not empty, show info message
	# otherwise indicate all tests are passed
	$(if $(shell if [ -s "./$(ERROR_LOG)" ]; then echo not empty; fi), \
		$(error $(shell cat $(ERROR_LOG))), \
		$(if $(shell if [ -s "./$(PARSER_ERROR_LOG)" ]; then echo not empty; fi), \
			$(info Ignored some parser errors, see $(PARSER_ERROR_LOG) for details),\
			$(info All Tests Passed)\
		)\
	)

.run_babel_tests:
	# run babel transpiler on js files, if js file is invalid, it writes all stderr to ERROR_LOG
	$(foreach t, $(TESTS), \
		$(eval ASSERT_FILE=$(subst /$(TESTS_PATH)/,/$(ASSERTS_PATH)/, $(patsubst %.js, %.txt, $(t)))) \
		$(shell $(BABEL) $(t) >> /dev/null 2>>$(ERROR_LOG);))
		$(if $(shell if [ -s "./$(ERROR_LOG)" ]; then echo not empty; fi),\
		 	$(error $(shell cat $(ERROR_LOG))),\
		  $(info All JS files are valid))

.run_lexer_tests: .build_lexer_test
	$(info Running Lexer Tests)
	# run lexer tests, use diff to compare lexer dump to expected output
	$(foreach t, $(TESTS), \
		$(eval ASSERT_FILE=$(subst /$(TESTS_PATH)/,/$(ASSERTS_PATH)/, $(patsubst %.js, %.txt, $(t)))) \
		$(shell diff $(ASSERT_FILE) <(./tests/test_lex < $(t))>> $(TEMP_ERROR_LOG) 2>&1;\
			if [ -s "./$(TEMP_ERROR_LOG)" ]; then echo $(t) >> $(ERROR_LOG); \
				cat $(TEMP_ERROR_LOG) >> $(ERROR_LOG); fi; \
				rm -f $(TEMP_ERROR_LOG);\
		)\
	)

.run_parser_tests: .build_parser_test
	$(info Running Parser Tests)
	# test parseable tests, log any error to ERROR_LOG
	$(foreach t, $(wildcard ./$(TESTS_ROOT)/parseable/$(TESTS_PATH)/*.js), \
		$(shell touch $(TEMP_ERROR_LOG); ./tests/test_parser < $(t) >> $(TEMP_ERROR_LOG) 2>&1;\
		if [ -s "./$(TEMP_ERROR_LOG)" ]; then echo $(t) >> $(ERROR_LOG); \
			cat $(TEMP_ERROR_LOG) >> $(ERROR_LOG); fi; rm -f $(TEMP_ERROR_LOG);\
		)\
	)
	# test unparseable tests, if tests are parseable, show warning to indicate tests are parseable
	$(foreach t, $(wildcard ./$(TESTS_ROOT)/unparseable/$(TESTS_PATH)/*.js), \
		$(if $(shell touch $(TEMP_ERROR_LOG); ./tests/test_parser < $(t) >> $(TEMP_ERROR_LOG) 2>&1;\
			if [ -s "./$(TEMP_ERROR_LOG)" ]; then echo not empty; fi;\
				echo $(t) >> $(PARSER_ERROR_LOG); \
				cat $(TEMP_ERROR_LOG) >> $(PARSER_ERROR_LOG);\
				rm -f $(TEMP_ERROR_LOG);\
			),,\
			 $(warning $(t) is parseable, consider moving it to /parseable/)\
		)\
	)
