// scanner.h - The scanner.
// Written by pianophage
// 8/17/15

#ifndef PIANOPHAGE_CALCULATOR_SCANNER_H
#define PIANOPHAGE_CALCULATOR_SCANNER_H

#include <vector>
#include <string>

#include "token.h"

class Scanner {
public:
    typedef std::vector<Token>::size_type size_type;

    Scanner(const std::string & input) : input { &input } {}
    
    const Token & operator[](size_type);
    
private:
    // The input string.
    const std::string * input;
    
    // The index of the current character we are looking at in the input string.
    std::string::size_type i = 0;
    
    // The token stream.
    std::vector<Token> tokenStream;
    
    void parseNextToken();
};

#endif  // PIANOPHAGE_CALCULATOR_SCANNER_H
