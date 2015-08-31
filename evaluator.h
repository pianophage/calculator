// evaluator.h - The evaluator.
// Written by pianophage
// 8/17/15

#ifndef PIANOPHAGE_CALCULATOR_EVALUATOR_H
#define PIANOPHAGE_CALCULATOR_EVALUATOR_H

#include <string>

#include "scanner.h"
#include "token.h"

class Evaluator {
public:
    Evaluator(const std::string & input) : scanner { input }, scanIndex { 0 } {}

    bool eval(long double &);

private:
    Scanner scanner;
    Scanner::size_type scanIndex;
    
    bool expr(long double &);
    bool plusTerm(long double &);
    bool minusTerm(long double &);
    bool term(long double &);
    bool timesFactor(long double &);
    bool dividedByFactor(long double &);
    bool factor(long double &);
    bool parenExpr(long double &);
    
    bool expect(Token::TokenType);
    bool expect(Token::TokenType, long double &);
};

#endif  // PIANOPHAGE_CALCULATOR_EVALUATOR_H
