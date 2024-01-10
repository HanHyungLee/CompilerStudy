//
//  main.cpp
//  CompilerStudy
//
//  Created by Lee Hanhyung on 11/29/23.
//

#include <iostream>
#include "main.h"

auto main()->int {
    string sourceCode = R""""(
            function main() {
                printLine 'Hello, World!';
                printLine 1 + 2 * 3;
            }
        )"""";
    auto tokenList = scan(sourceCode);
    auto syntaxTree = parse(tokenList);
    
    printTokenList(tokenList);
    
    return 0;
}
