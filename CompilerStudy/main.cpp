//
//  main.cpp
//  CompilerStudy
//
//  Created by Lee Hanhyung on 11/29/23.
//

#include <iostream>
#include "main.h"

// chapter 3. 구문 분석
string sourceCode = R""""(
        function main() {
            printLine 'Hello, World!';
            printLine 1 + 2 * 3;
        }
    )"""";

auto main()->int {
    auto tokenList = scan(sourceCode);
    auto syntaxTree = parse(tokenList);
    
    printTokenList(tokenList);    // 3장 구문 분석
//    interpret(syntaxTree);      // 4장 인터프리터
    
    return 0;
}
