//
//  Parse.hpp
//  CompilerStudy
//
//  Created by Hanhyung Lee on 2023/12/18.
//

#ifndef Parse_hpp
#define Parse_hpp

#include <stdio.h>
#include <vector>

#include "Kind.h"
#include "Node.hpp"

using namespace std;

static vector<Token>::iterator current;

auto parseFunction() -> Function*;
auto skipCurrent(Kind kind)->void;
auto parseBlock()->vector<Statement*>;
auto parseVariable()->Variable*;
auto parseExpression()->Expression*;
auto parseAssignment()->Expression*;
auto parseOr()->Expression*;

#endif /* Parse_hpp */
