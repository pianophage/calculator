// calculator.cc - A calculator.
// Written by pianophage
// 8/16/15

#include <cstdlib>
#include <iostream>
#include <string>

#include <readline/readline.h>
#include <readline/history.h>

#include "evaluator.h"

using namespace std;

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
