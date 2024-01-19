#pragma once
#include <vector>
#include <iomanip>
#include <iostream>
#include <map>
//#include "Token.h"
#include "Kind.h"
#include "Scanner.h"
#include "Node.hpp"
#include "Parse.hpp"
#include "Chapter4SourceCode.h"

using std::vector;
using std::string;

auto scan(string)->vector<Token>;
auto printTokenList(vector<Token>) -> void;
auto parse(vector<Token>) -> Program*;
auto interpret(Program*)->void;
