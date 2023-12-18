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
#include "Kind.h"

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

struct Print: Statement {
    bool lineFeed = false;
    vector<Expression*> arguments;
};

struct ExpressionStatement: Statement {
    Expression* expression;
};

struct Or: Expression {
    Expression* lhs;
    Expression* rhs;
};

struct And: Expression {
    Expression* lhs;
    Expression* rhs;
};

struct Relational: Expression {
    Kind kind;
    Expression* lhs;
    Expression* rhs;
};

struct Arithmetic: Expression {
    Kind kind;
    Expression* lhs;
    Expression* rhs;
};

struct Unary: Expression {
    Kind kind;
    Expression* sub;
};

struct Call: Expression {
    Expression* sub;
    vector<Expression*> arguments;
};

struct GetElement: Expression {
    Expression* sub;
    Expression* index;
};

struct SetElement: Expression {
    Expression* sub;
    Expression* index;
    Expression* value;
};

struct GetVariable: Expression {
    string name;
};

struct SetVariable: Expression {
    string name;
    Expression* value;
};

struct NullLiteral: Expression {};
struct BooleanLiteral: Expression {
    bool value = false;
};
struct NumberLiteral: Expression {
    double value = 0.0;
};
struct StringLiteral: Expression {
    string value;
};

struct ArrayLiteral: Expression {
    vector<Expression*> values;
};

struct MapLiteral: Expression {
    map<string, Expression*> values;
};

#endif /* Node_hpp */
