#pragma once
#include <vector>
#include <iomanip>
#include <iostream>
#include <map>
//#include "Token.h"
#include "Kind.h"
#include "Scanner.h"

using std::vector;
using std::string;

auto scan(string)->vector<Token>;
auto printTokenList(vector<Token>) -> void;
