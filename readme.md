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


### Error Logs
| Log file  | What's in it                                    | Importance |
|-----------|---------------                                  |------------|
| error.log | lexer error, parser error for /parseable/ tests | You need to fix these to have a successful build |
| error_parser.log | parser error for /unparseable/ tests     | You want to fix these eventually |
| temperror.log | temporary error file                        | Ignore it |
| grammar.dot, grammar.output | Bison logs                    | Useful for fixing parser errors |
