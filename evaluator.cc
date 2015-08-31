// evaluator.cc - The evaluator.
// Written by pianophage
// 8/17/15

#include <iostream>

#include "evaluator.h"

using namespace std;

// Evaluate a line of input as an arithmetic expression and return the result.
bool Evaluator::eval(long double & value) {
    return expr(value) && expect(Token::EOI);
}

// expr = term (plusTerm | minusTerm)*
bool Evaluator::expr(long double & value) {
    if (term(value)) {
        for (;;) {
            long double rightValue;
            
            if (plusTerm(rightValue)) {
                value += rightValue;
            } else if (minusTerm(rightValue)) {
                value -= rightValue;
            } else {
                break;
            }
        }
        
        return true;
    } else {
        return false;
    }
}

// plusTerm = '+' term
bool Evaluator::plusTerm(long double & value) {
    auto rewindPoint { scanIndex };
    
    if (expect(Token::PLUS) && term(value)) {
        return true;
    } else {
        scanIndex = rewindPoint;
        
        return false;
    }
}

// minusTerm = '-' term
bool Evaluator::minusTerm(long double & value) {
    auto rewindPoint { scanIndex };
    
    if (expect(Token::MINUS) && term(value)) {
        return true;
    } else {
        scanIndex = rewindPoint;
        
        return false;
    }
}

// term = factor (timesFactor | dividedByFactor)*
bool Evaluator::term(long double & value) {
    if (factor(value)) {
        for (;;) {
            long double rightValue;
            
            if (timesFactor(rightValue)) {
                value *= rightValue;
            } else if (dividedByFactor(rightValue)) {
                value /= rightValue;
            } else {
                break;
            }
        }
        
        return true;
    } else {
        return false;
    }
}

// timesFactor = '*' factor
bool Evaluator::timesFactor(long double & value) {
    auto rewindPoint { scanIndex };
    
    if (expect(Token::TIMES) && factor(value)) {
        return true;
    } else {
        scanIndex = rewindPoint;
        
        return false;
    }
}

// dividedByFactor = '/' factor
bool Evaluator::dividedByFactor(long double & value) {
    auto rewindPoint { scanIndex };
    
    if (expect(Token::DIVIDED_BY) && factor(value)) {
        return true;
    } else {
        scanIndex = rewindPoint;
        
        return false;
    }
}

// factor = NUMBER | parenExpr
bool Evaluator::factor(long double & value) {
    return expect(Token::NUMBER, value) || parenExpr(value);
}

// parenExpr = '(' expr ')'
bool Evaluator::parenExpr(long double & value) {
    auto rewindPoint { scanIndex };
    
    if (expect(Token::OPEN_PAREN) && expr(value) && expect(Token::CLOSE_PAREN)) {
        return true;
    } else {
        scanIndex = rewindPoint;
        
        return false;
    }
}

// If the next token parsed matches the given type, return true. Otherwise, return false.
bool Evaluator::expect(Token::TokenType expected) {
    long double ignored;
    
    return expect(expected, ignored);
}

// If the next token parsed matches the given type, set 'value' to the token value and return true. Otherwise, leave
// 'value' undefined and return false.
bool Evaluator::expect(Token::TokenType expected, long double & value) {
    const Token & token { scanner[scanIndex] };
    if (token.type == expected) {
        value = token.value;
        ++scanIndex;
        
        return true;
    } else {
        return false;
    }
}





