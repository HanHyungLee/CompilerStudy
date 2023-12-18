//
//  Kind.h
//  CompilerStudy
//
//  Created by Lee Hanhyung on 11/29/23.
//

#pragma once

//#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include "Token.h"

struct Token;

using std::string;
using std::ostream;
using std::vector;

enum class Kind {
    Unknown, EndOfToken,
    
    NullLiteral,
    TrueLiteral, FalseLiteral,
    NumberLiteral, StringLiteral,
    Identifier,
    
    Function, Return,
    Variable,
    For, Break, Continue,
    If, Elif, Else,
    Print, PrintLine,
    
    LogicalAnd, LogicalOr,
    Assignment,
    Add, Subtract,
    Multiply, Divide, Modulo,
    Equal, NotEqual,
    LessThan, GreaterThan,
    LessOrEqual, GreaterOrEqual,
    
    Comma, Colon, Semicolon,
    LeftParen, RightParen,
    LeftBrace, RightBrace,
    LeftBraket, RightBraket,
};

struct Token {
    Kind kind = Kind::Unknown;
    string string;
};

auto toKind(string) -> Kind;
auto printTokenList(vector<Token>) -> void;


