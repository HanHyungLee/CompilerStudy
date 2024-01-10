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
#include <map>
#include <any>
#include "Kind.h"

using namespace std;

struct Program {
    vector<struct Function*> functions;
};

struct Statement {
    virtual auto print(int)->void = 0;
    virtual auto interpret()->void = 0;
};
struct Expression {
    virtual auto print(int)->void = 0;
    virtual auto interpret()->any = 0;
};

struct Function: Statement {
    string name;
    vector<string> parameters;
    vector<Statement*> block;
    auto print(int)->void;
    auto interpret()->void;
};

struct Return: Statement {
    Expression* expression;
    auto print(int)->void;
    auto interpret()->void;
};

struct Variable: Statement {
    string name;
    Expression* expression;
    auto print(int)->void;
    auto interpret()->void;
};

struct For: Statement {
    Variable* variable;
    Expression* condition;
    Expression* expression;
    vector<Statement*> block;
    auto print(int)->void;
    auto interpret()->void;
};

struct Break: Statement {
    auto print(int)->void;
    auto interpret()->void;
};
struct Continue: Statement {
    auto print(int)->void;
    auto interpret()->void;
};

struct If: Statement {
    vector<Expression*> conditions;
    vector<vector<Statement*>> blocks;
    vector<Statement*> elseBlock;
    auto print(int)->void;
    auto interpret()->void;
};

struct Print: Statement {
    bool lineFeed = false;
    vector<Expression*> arguments;
    auto print(int)->void;
    auto interpret()->void;
};

struct ExpressionStatement: Statement {
    Expression* expression;
    auto print(int)->void;
    auto interpret()->void;
};

struct Or: Expression {
    Expression* lhs;
    Expression* rhs;
    auto print(int)->void;
    auto interpret()->any;
};

struct And: Expression {
    Expression* lhs;
    Expression* rhs;
    auto print(int)->void;
    auto interpret()->any;
};

struct Relational: Expression {
    Kind kind;
    Expression* lhs;
    Expression* rhs;
    auto print(int)->void;
    auto interpret()->any;
};

struct Arithmetic: Expression {
    Kind kind;
    Expression* lhs;
    Expression* rhs;
    auto print(int)->void;
    auto interpret()->any;
};

struct Unary: Expression {
    Kind kind;
    Expression* sub;
    auto print(int)->void;
    auto interpret()->any;
};

struct Call: Expression {
    Expression* sub;
    vector<Expression*> arguments;
    auto print(int)->void;
    auto interpret()->any;
};

struct GetElement: Expression {
    Expression* sub;
    Expression* index;
    auto print(int)->void;
    auto interpret()->any;
};

struct SetElement: Expression {
    Expression* sub;
    Expression* index;
    Expression* value;
    auto print(int)->void;
    auto interpret()->any;
};

struct GetVariable: Expression {
    string name;
    auto print(int)->void;
    auto interpret()->any;
};

struct SetVariable: Expression {
    string name;
    Expression* value;
    auto print(int)->void;
    auto interpret()->any;
};

struct NullLiteral: Expression {
    auto print(int)->void;
    auto interpret()->any;
};
struct BooleanLiteral: Expression {
    bool value = false;
    auto print(int)->void;
    auto interpret()->any;
};
struct NumberLiteral: Expression {
    double value = 0.0;
    auto print(int)->void;
    auto interpret()->any;
};
struct StringLiteral: Expression {
    string value;
    auto print(int)->void;
    auto interpret()->any;
};

struct ArrayLiteral: Expression {
    vector<Expression*> values;
    auto print(int)->void;
    auto interpret()->any;
};

struct MapLiteral: Expression {
    map<string, Expression*> values;
    auto print(int)->void;
    auto interpret()->any;
};

#endif /* Node_hpp */
