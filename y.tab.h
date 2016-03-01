enum yytokentype {

    // KEYWORDS
    BREAK,
    CASE,
    CATCH,
    CLASS,
    CONST,
    CONTINUE,
    DEBUGGER,
    DEFAULT,
    DELETE,
    DO,
    ELSE,
    EXPORT,
    EXTENDS,
    FINALLY,
    FOR,
    FUNCTION,
    IF,
    IMPORT,
    IN,
    INSTANCEOF,
    NEW,
    RETURN,
    SUPER,
    SWITCH,
    THIS,
    THROW,
    TRY,
    TYPEOF,
    VAR,
    VOID,
    WHILE,
    WITH,
    YIELD,

    // FUTURE RESERVED WORDS
    ENUM,
    AWAIT,
    IMPLEMENTS,
    INTERFACE,
    PACKAGE,
    PRIVATE,
    PROTECTED,
    PUBLIC,

    // LITERALS
    LITERAL_NULL,                       // Null
    LITERAL_TRUE,                       // true
    LITERAL_FALSE,                      // false
    LITERAL_UNDEFINED,                  // undefined
    LITERAL_NAN,                        // NaN

    // UNARY OPERATORS
    UNARY_ADD,                          // ++
    UNARY_SUBTRACT,                     // --
    LOGICAL_NOT,                        // !

    // MULTIPLICATIVE OPERATORS
    MULTIPLY,                           // *
    DIVIDE,                             // /
    MODULO,                             // %

    // ADDITIVE OPERATORS
    ADD,                                // +
    SUBTRACT,                           // -

    // EQUALITY OPERATORS
    EQUAL,                              // ==
    NOT_EQUAL,                          // !=
    EXACTLY_EQUAL,                      // ===
    NOT_EXACTLY_EQUAL,                  // !==

    // BITWISE SHIFT OPERATORS
    LEFT_SHIFT,                         // <<
    SIGNED_RIGHT_SHIFT,                 // >>
    UNSIGNED_RIGHT_SHIFT,               // >>>

    // BINARY LOGICAL OPERATORS
    QUESTION_MARK,                      // ?
    COLON,                              // :
    LOGICAL_AND,                        // &&
    LOGICAL_OR,                         // ||

    // BINARY BITWISE OPERATORS
    BITWISE_AND,                        // &
    BITWISE_OR,                         // |
    BITWISE_XOR,                        // ^

    // UNARY BITWISE OPERATORS
    BITWISE_NOT,                        // ~

    // ASSIGNMENT OPERATORS
    ASSIGNMENT,                         // =
    ADDITION_ASSIGNMENT,                // +=
    SUBTRACTION_ASSIGNMENT,             // -=
    MULTIPLICATION_ASSIGNMENT,          // *=
    DIVISION_ASSIGNMENT,                // /=
    REMAINDER_ASSIGNMENT,               // %=
    EXPONENTIATION_ASSIGNMENT,          // **=
    LEFT_SHIFT_ASSIGNMENT,              // <<=
    RIGHT_SHIFT_ASSIGNMENT,             // >>=
    UNSIGNED_RIGHT_SHIFT_ASSIGNMENT,    // >>>=
    BITWISE_AND_ASSIGNMENT,             // &=
    BITWISE_XOR_ASSIGNMENT,             // ^=
    BITWISE_OR_ASSIGNMENT,              // |=


    // GENERAL PUNCTUATORS
    RIGHT_PAREN,                        // (
    LEFT_PAREN,                         // )
    RIGHT_BRACE,                        // {
    LEFT_BRACE,                         // }
    RIGHT_BRACKET,                      // [
    LEFT_BRACKET,                       // ]
    COMMA,                              // ,
    FULL_STOP,                          // .
    ELLIPSIS,                           // ...
    SEMICOLON,                          // ;

    // STRINGS
    DOUBLE_QUOTE,                       // "
    SINGLE_QUOTE                        // '

};