// token.h - Tokens, spit out from the scanner and consumed by the parser.
// Written by pianophage
// 8/18/15

#ifndef PIANOPHAGE_CALCULATOR_TOKEN_H
#define PIANOPHAGE_CALCULATOR_TOKEN_H

// This class is immutable.
struct Token {
    enum TokenType {
        ERROR = 0,
        EOI = 1,
        OPEN_PAREN = 2,
        CLOSE_PAREN = 3,
        PLUS = 4,
        MINUS = 5,
        TIMES = 6,
        DIVIDED_BY = 7,
        NUMBER = 8
    };
    
    // For non-numeric tokens.
    Token(TokenType type) : type { type }, value { 0.0 } {}
    
    // For numeric tokens.
    Token(TokenType type, long double value) : type { type }, value { value } {}
    
    // The type of this token, from the above enum.
    const TokenType type;
    
    // The value of the token. Only numbers have values so far.
    const long double value;
};

#endif  // PIANOPHAGE_CALCULATOR_TOKEN_H
