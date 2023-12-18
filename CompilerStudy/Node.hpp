//
//  Node.hpp
//  CompilerStudy
//
//  Created by Hanhyung Lee on 2023/12/18.
//

#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>
#include <vector>
#include <iostream>

using namespace std;

struct Program {
    vector<struct Function*> functions;
};

struct Statement {};
struct Expression {};

struct Function: Statement {
    string name;
    vector<string> parameters;
    vector<Statement*> block;
};

struct Return: Statement {
    Expression* expression;
};

struct Variable: Statement {
    string name;
    Expression* expression;
};

struct For: Statement {
    Variable* variable;
    Expression* condition;
    Expression* expression;
    vector<Statement*> block;
};

struct Break: Statement {};
struct Continue: Statement {};

struct If: Statement {
    vector<Expression*> conditions;
    vector<vector<Statement*>> blocks;
    vector<Statement*> elseBlock;
};

#endif /* Node_hpp */
