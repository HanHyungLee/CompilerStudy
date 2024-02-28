#pragma once
#include <vector>
#include <iomanip>
#include <iostream>
#include <map>
//#include "Token.h"
#include "Chapter4SourceCode.h"
#include "Kind.h"
#include "Scanner.h"
#include "Node.hpp"
#include "Parse.hpp"
#include "Code.hpp"
#include "Machine.hpp"

using namespace std;

auto scan(string)->vector<Token>;
auto printTokenList(vector<Token>) -> void;
auto parse(vector<Token>) -> Program*;
auto interpret(Program*)->void;
auto generate(Program*)->tuple<vector<Code>, map<string, size_t>>;
auto printObjectCode(tuple<vector<Code>, map<string, size_t>> objectCode)->void;
auto execute(tuple<vector<Code>, map<string, size_t>>)->void;
