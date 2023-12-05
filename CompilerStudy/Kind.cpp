//
//  Kind.cpp
//  CompilerStudy
//
//  Created by Lee Hanhyung on 11/29/23.
//

#include "Kind.h"
#include <iomanip>
#include <map>

//using std::map;
//using std::setw;
//using std::left;
//using std::cout;
//using std::endl;
using namespace std;


static map<string, Kind> stringToKind = {
    {"#unknown",    Kind::Unknown},
    {"#EndOfToken", Kind::EndOfToken},
    
    {"null",        Kind::NullLiteral},
    {"true",        Kind::TrueLiteral},
    {"false",       Kind::FalseLiteral},
    {"#Number",     Kind::NumberLiteral},
    {"#String",     Kind::StringLiteral},
    {"#identifier", Kind::Identifier},
    
    {"function",    Kind::Function},
    {"return",      Kind::Return},
    {"var",         Kind::Variable},
    {"for",         Kind::For},
    {"break",       Kind::Break},
    {"continue",    Kind::Continue},
    {"if",          Kind::If},
    {"elif",        Kind::Elif},
    {"else",        Kind::Else},
    {"print",       Kind::Print},
    {"printLine",   Kind::PrintLine},
    
    {"and",         Kind::LogicalAnd},
    {"or",          Kind::LogicalOr},
    
    {"=",           Kind::Assignment},
    
    {"+",           Kind::Add},
    {"-",           Kind::Subtract},
    {"*",           Kind::Multiply},
    {"/",           Kind::Divide},
    {"%",           Kind::Modulo},
    
    {"==",          Kind::Equal},
    {"!=",          Kind::NotEqual},
    {"<",           Kind::LessThan},
    {">",           Kind::GreaterThan},
    {"<=",          Kind::LessOrEqual},
    {">=",          Kind::GreaterOrEqual},
    
    {",",           Kind::Comma},
    {":",           Kind::Colon},
    {";",           Kind::Semicolon},
    {"(",           Kind::LeftParen},
    {")",           Kind::RightParen},
    {"{",           Kind::LeftBrace},
    {"}",           Kind::RightBrace},
    {"[",           Kind::LeftBraket},
    {"]",           Kind::RightBraket},
};

static auto kindToString = [] {
    map<Kind, string> result;
    for (auto& [key, value] : stringToKind)
        result[value] = key;
    return result;
}();

auto toKind(string string)->Kind {
    if (stringToKind.count(string))
        return stringToKind.at(string);
    return Kind::Unknown;
}

auto toString(Kind type)->string {
    if (kindToString.count(type))
        return kindToString.at(type);
    return "";
}

auto operator<<(ostream& stream, Token& token)->ostream& {
    return stream << setw(12) << left << toString(token.kind) << token.string;
}

auto printTokenList(vector<Token> tokenList)->void {
    cout << setw(12) << left << "KIND" << "STRING" << endl;
    cout << string(23, '-') << endl;
    for (auto& token: tokenList)
        cout << token << endl;
}
