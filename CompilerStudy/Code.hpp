//
//  Code.hpp
//  CompilerStudy
//
//  Created by Hanhyung Lee on 2024/01/19.
//

#ifndef Code_hpp
#define Code_hpp

#include <stdio.h>
#include "Instruction.hpp"
#include "Kind.h"
#include <any>
#include <iostream>
#include <iomanip>
#include <map>

using namespace std;

struct Code {
    Instruction instruction;
    any operand;
};

// 책에 빠진 내용 붙여넣어야 한다.
auto toString(Instruction)->string;

// 책에 빠진 내용 붙여넣어야 한다.
auto operator<<(ostream&, Code&)->ostream&;

#endif /* Code_hpp */
