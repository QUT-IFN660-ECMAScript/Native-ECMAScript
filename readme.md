## Dependencies
- gcc/g++ 5+ on ubuntu, on OSX 4.2.1 is sufficient
- flex 2.5+
- bison 2.3+ - 3.0 will still work but gives warnings
- babel-cli from npm, installed with -g - if you want to run JS validations

## Project Structure
```
|-- ast                    # contains AST node classes
|-- tests            
|   |-- parseable
|   |   |-- lexer-assert   # contains expected lexer output for parseable/test tests
|   |   |-- test           # tests in this folder must be
|   |                        valid javascript and passes all tests
|   |-- unparseable
|       |-- lexer-assert   # contains expected lexer output for unparseable/test tests
|       |-- test           # tests in this folder must be
|                            valid javascript, passes lexer tests,
|                            but does not need to be parseable
|-- .travis.yml            # travis ci build configuration
|-- grammar.l              # lexer/flex grammar
|-- grammar.y              # bison/parser grammar
...
```

## Build Commands

Just build the compiler
```
make
```


Build compiler, run tests, but without js file validations (faster)
```
make test
```


Build compiler, run tests, and js file validations
```
make test_all
```


Build compiler, and run lexer tests only
```
make test_lexer
```


Build compiler, and run parser tests only
```
make test_parser
```


## Error Logs
| Log  | What's in it                                         | What's it for |
|-----------|---------------                                  |------------|
| error.log | lexer error, parser error for /parseable/ tests | You need to fix these to have a successful build |
| error_parser.log | parser error for /unparseable/ tests     | You want to fix these eventually |
| temperror.log | temporary error file                        | Ignore it |
| grammar.dot, grammar.output | Bison logs                    | Useful for fixing parser errors |
