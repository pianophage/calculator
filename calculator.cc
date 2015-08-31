// calculator.cc - A calculator.
// Written by pianophage
// 8/16/15

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <readline/readline.h>
#include <readline/history.h>

using namespace std;

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

// An evaluator for simple arithmetic, floating-point expressions.
class Evaluator {
public:
    // Construct a new evaluator for the given input expression.
    Evaluator(const string & input) : scanner { input }, scanIndex { 0 } {}

    // If the input is parseable as an arithmetic expression, evaluate it, return its value in 'value', and return
    // true. Otherwise, leave 'value' undefined and return false.
    bool eval(long double & value) {
        return expr(value) && expect(Token::EOI);
    }

private:
    Scanner scanner;
    Scanner::size_type scanIndex;
    
    bool expr(long double & value);
    bool plusTerm(long double & value);
    bool minusTerm(long double & value);
    bool term(long double & value);
    bool timesFactor(long double & value);
    bool dividedByFactor(long double & value);
    bool factor(long double & value);
    bool parenExpr(long double & value);
    
    bool expect(Token::TokenType expected);
    bool expect(Token::TokenType expected, long double & value);
};

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

int main() {
    for (;;) {
        char * line_cstr = readline(">> ");
        if (! line_cstr) {
            // Ctrl-D, exit.
            cout << endl;
            
            break;
        }
        
        string line { line_cstr };
        delete line_cstr;
        
        if (! line.empty()) {
            add_history(line.c_str());
        } else {
            // Ignore empty lines.
            continue;
        }
        
        Evaluator evaluator { line };
        long double result;
        
        if (evaluator.eval(result)) {
            cout << result << endl;
        } else {
            cerr << "parse error" << endl;
        }
    }
}
