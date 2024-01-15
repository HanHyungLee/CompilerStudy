//
//  main.cpp
//  CompilerStudy
//
//  Created by Lee Hanhyung on 11/29/23.
//

#include <iostream>
#include "main.h"

// chapter 3. 구문 분석
//string sourceCode = R""""(
//        function main() {
//            printLine 'Hello, World!';
//            printLine 1 + 2 * 3;
//        }
//    )"""";

// chapter 4. 인터프리터
//string sourceCode = R""""(
//            function main() {
//                printLine 'Hello, World!';
//            }
//        )"""";

// 사칙연산 테스트
//string sourceCode = R""""(
//            function main() {
//                printLine 1 * 2 + 3 * 4;
//                printLine '123' * '456';
//                printLine 12 / 3;
//                printLine 12 % 10;
//                printLine 12 - 3;
//            }
//        )"""";

// Or, And
//string sourceCode = R""""(
//            function main() {
//                printLine true or 'Hello, World';
//                printLine false or 'Hello, World';
//                printLine true and 'Hello, World';
//                printLine false and 'Hello, World';
//            }
//        )"""";

// 지역, 전역 변수
//string sourceCode = R""""(
//            function main() {
//                global = 4;
//                var local = 13;
//                global = local = 7;
//                printLine 'global: ', global;
//                printLine 'local: ', local;
//            }
//        )"""";

// for
//string sourceCode = R""""(
//            function main() {
//                for i=0, i<3, i=i+1 {
//                    printLine 'i: ', i;
//                }
//            }
//        )"""";

// if
//string sourceCode = R""""(
//            function main() {
//                for i=0, i<5, i=i+1 {
//                    if i == 1 {
//                        printLine 'one';
//                    } elif i == 2 {
//                        printLine 'two';
//                    } elif i == 3 {
//                        printLine 'three';
//                    } else {
//                        printLine i;
//                    }
//                }
//            }
//        )"""";

// continue
//string sourceCode = R""""(
//            function main() {
//                for i=0, i<3, i=i+1 {
//                    if i == 1 {
//                        continue;
//                    }
//                    printLine 'i: ', i;
//                }
//            }
//        )"""";

// function
//string sourceCode = R""""(
//            function main() {
//                sayHoHoHo();
//            }
//
//            function sayHoHoHo() {
//                printLine 'Ho! Ho! Ho!';
//            }
//        )"""";

// function parameter
//string sourceCode = R""""(
//            function main() {
//                add(1, 3);
//            }
//
//            function add(a, b) {
//                printLine a + b;
//            }
//        )"""";

// function parameter2 -> ; 토큰이 필요합니다.
//string sourceCode = R""""(
//            function main() {
//                print getC(3, 4);
//            }
//
//            function getC(a, b) {
//                return a * a + b * b;
//            }
//        )"""";

//// 내장 함수 -> ; 토큰이 필요합니다.
//string sourceCode = R""""(
//            function main() {
//                print sqrt(getC(3, 4));
//            }
//
//            function getC(a, b) {
//                return a * a + b * b;
//            }
//        )"""";

// array
//string sourceCode = R""""(
//            function main() {
//                printLine [1, 2, 3];
//            }
//        )"""";

// array2
//string sourceCode = R""""(
//            function main() {
//                printLine ['first', 'second', 'third'][1];
//            }
//        )"""";

// array3
string sourceCode = R""""(
            function main() {
                var array = ['first', 'second', 'third'];
                array[1] = '2nd';
                printLine array[1];
            }
        )"""";

auto main()->int {
    auto tokenList = scan(sourceCode);
    auto syntaxTree = parse(tokenList);
    
//    printTokenList(tokenList);    // 3장 구문 분석
    interpret(syntaxTree);      // 4장 인터프리터
    
    return 0;
}
