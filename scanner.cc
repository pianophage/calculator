// scanner.cc - The scanner.
// Written by pianophage
// 8/17/15

#include <iostream>

#include "scanner.h"

using namespace std;

const Token & Scanner::operator[](Scanner::size_type i) {
    if (i == tokenStream.size()) {
        parseNextToken();
    }
    
    return tokenStream[i];
}

// From the current position in the input, parse the next token and push it to the end of the token stream.
void Scanner::parseNextToken() {
    char c = (*input)[i];
    
    // Eat whitespace.
    while (c == ' ' || c == '\t') {
        c = (*input)[++i];
    }
    
    /* check_open_paren: */ {
        if (c == '(') {
            tokenStream.emplace_back(Token::OPEN_PAREN);
            ++i;
            
            return;
        } else {
            goto check_close_paren;
        }
    }
    
    check_close_paren: {
        if (c == ')') {
            tokenStream.emplace_back(Token::CLOSE_PAREN);
            ++i;
            
            return;
        } else {
            goto check_plus;
        }
    }
    
    check_plus: {
        if (c == '+') {
            tokenStream.emplace_back(Token::PLUS);
            ++i;
            
            return;
        } else {
            goto check_minus;
        }
    }
    
    check_minus: {
        if (c == '-') {
            tokenStream.emplace_back(Token::MINUS);
            ++i;
            
            return;
        } else {
            goto check_times;
        }
    }
    
    check_times: {
        if (c == '*') {
            tokenStream.emplace_back(Token::TIMES);
            ++i;
            
            return;
        } else {
            goto check_divided_by;
        }
    }
    
    check_divided_by: {
        if (c == '/') {
            tokenStream.emplace_back(Token::DIVIDED_BY);
            ++i;
            
            return;
        } else {
            goto check_number;
        }
    }
    
    check_number: {
        long double value = 0.0;
        
        /* check_digit: */ {
            if ('0' <= c && c <= '9') {
                value = c - '0';
                
                c = (*input)[++i];
                
                goto check_digits;
            } else {
                goto check_eoi;
            }
        }
    
        check_digits: {
            if (! ('0' <= c && c <= '9')) {
                tokenStream.emplace_back(Token::NUMBER, value);
                
                return;
            } else {
                value = value * 10 + (c - '0');
            
                c = (*input)[++i];
            
                goto check_digits;
            }
        }
    }
    
    check_eoi: {
        if (c == '\0') {
            tokenStream.emplace_back(Token::EOI);
        } else {
            tokenStream.emplace_back(Token::ERROR);
        }
    }
}
