//
//  Interpreter.hpp
//  CompilerStudy
//
//  Created by Hanhyung Lee on 2024/01/10.
//

#ifndef Interpreter_hpp
#define Interpreter_hpp

#include <map>
#include <any>
#include <list>
#include <vector>
#include <functional>
#include "DataType.hpp"
#include <cmath>
#include "Node.hpp"

using namespace std;

auto Function::interpret()->void {
    for (auto& node: block)
        node->interpret();
}

auto Or::interpret()->any {
    return isTrue(lhs->interpret()) ? true : rhs->interpret();
}

static map<string, Function*> functionTable;

auto interpret(Program * program)->void {
    for (auto& node: program->functions)
        functionTable[node->name] = node;
    
    if (functionTable["main"] == nullptr)
        return;
    functionTable["main"]->interpret();
}

auto Print::interpret()->void {
    for (auto& node: arguments) {
        auto value = node->interpret();
        cout << value;
    }
    if (lineFeed) cout << endl;
}

#endif /* Interpreter_hpp */
