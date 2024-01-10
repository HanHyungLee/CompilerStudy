//
//  Kind.h
//  CompilerStudy
//
//  Created by Lee Hanhyung on 11/29/23.
//

#pragma once

#include <vector>
#include <stdio.h>
#include <string>
#include "Token.h"

struct Token;

using namespace std;

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
auto toString(Kind type)->string;
auto printTokenList(vector<Token>) -> void;
auto operator<<(ostream& stream, Token& token)->ostream&;

