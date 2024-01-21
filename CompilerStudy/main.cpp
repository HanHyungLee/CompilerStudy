//
//  main.cpp
//  CompilerStudy
//
//  Created by Lee Hanhyung on 11/29/23.
//

#include "main.h"

// chapter 3. 구문 분석
//string sourceCode = R""""(
//        function main() {
//            printLine 'Hello, World!';
//            printLine 1 + 2 * 3;
//        }
//    )"""";

// chapter 5. 코드 생성

/*
 출력 결과 5-2, 5-3, 5-4
 
 FUNCTION   ADDRESS
 ------------------
 main       3

 ADDR INSTRUCTION    OPERAND
 ------------------------------------
    0 GetGlobal      "main"             -> 주소 0의 GetGlobal "main" 코드는 문자열 "main"으로 함수 테이블을 검색해 main()함수의 주소 3을 피연산자 스택에 넣는다.
    1 Call           [0]                -> Call [0] 코드로 피연산자 스택에서 함수의 주소를 꺼내 해당 주소로 점프한다.
    2 Exit
    3 PushString     "Hello, World!"    -> 주소 3의 PushString 으로 "Hello, World!" 문자열을 넣는다.
    4 Print          [1]                -> 주소 4의 Print [1] 코드는 피연산자 스택에서 "Hello, World!"를 꺼내 콘솔에 출력한다.
    5 Return                            -> 주소 5의 Return 코드는 함수를 호추루한 코드의 바로 다음 코드로 점프하고, 주소 2의 Exit 코드로 프로그램을 종료한다.
 
 [ 피연산자 스택 ]
 1. [3]
 2. []
 3. ["Hello, World!"]
 4. []
 5. []
 */
//string sourceCode = R""""(
//        function main() {
//            print 'Hello, World!';
//        }
//    )"""";

/*
 출력 결과 5-5
 
 FUNCTION   ADDRESS
 ------------------
 main       3

 ADDR INSTRUCTION    OPERAND
 ------------------------------------
    0 GetGlobal      "main"
    1 Call           [0]
    2 Exit
    3 PushNumber     1
    4 PushNumber     2
    5 Multiply
    6 PushNumber     3
    7 PushNumber     4
    8 Multiply
    9 Add
   10 Print          [1]
   11 Return
 
 [ 피연산자 스택 ]
 1. [1]
 2. [1, 2]
 3. [2,]
 4. [2, 3]
 5. [2, 3, 4]
 6. [2, 12]
 7. [14]
 8. []
 */
//string sourceCode = R""""(
//        function main() {
//            print 1 * 2 + 3 * 4;
//        }
//    )"""";

/*
 출력 결과 5-6, 5-7
 */
//string sourceCode = R""""(
//        function main() {
//            1 + 2;
//        }
//    )"""";

/*
 출력 결과 5-8
 
 FUNCTION   ADDRESS
 ------------------
 main       3

 ADDR INSTRUCTION    OPERAND
 ------------------------------------
    0 GetGlobal      "main"
    1 Call           [0]
    2 Exit
    3 PushBoolean    false
    4 LogicalOr      [8]    -> 주소 4의 LogicalOr [8] 코드가 피연산자 스택에 있는 값이 참이면 주소 8로 점프하고 아니라면 값을 꺼내 버린다.
    5 PushNumber     1
    6 PushNumber     2
    7 Add
    8 Print          [1]    -> 주소 8의 Print [1] 코드는 피연산자 스택에서 값을 하나 꺼내 콘솔에 출력한다. 여기서는 3이 출력된다. 만약 or 연산자의 왼쪽 식이 false가 아닌 true였다면 주소 4의 LogicalOr [8] 코드는 주소 8로 점프하고, 피연산자 스택에는 true가 남아 있다.
    9 Return
 
 [ 피연산자 스택 ]
 1. [false]
 2. []
 3. [1]
 4. [1, 2]
 5. [3]
 6. [true]
 */
//string sourceCode = R""""(
//        function main() {
//            print false or 1 + 2;
//        }
//    )"""";

/*
 출력 결과 5-9
 
 FUNCTION   ADDRESS
 ------------------
 main       3

 ADDR INSTRUCTION    OPERAND
 ------------------------------------
    0 GetGlobal      "main"
    1 Call           [0]
    2 Exit
    3 Alloca         [2]        -> Alloca [2] 코드는 함수를 실행하기 위해 필요한 메모리 공간을 2만큼 할당한다.
    4 PushString     "first"
    5 SetLocal       [0]        -> 주소 5의 SetLocal[0] 코드는 피연산자 스택의 값을 함수 공간의 첫 번째에 저장한다.
    6 PopOperand                -> 주소 6의 PopOperand 코드는 피연산자 스택의 값을 꺼내어 버린다.
    7 PushString     "second"
    8 SetLocal       [1]
    9 PopOperand
   10 Return
 
 [ 피연산자 스택 ] | [ 함수 공간 ]
 1. [,] | [,]
 2. ["first",] | [,]
 3. ["first",] | ["first",]
 4. [,] | ["first",]
 5. [,] | ["first", "second"]
 */
//string sourceCode = R""""(
//        function main() {
//            var a = 'first';
//            var b = 'second';
//        }
//    )"""";

/*
 출력 결과 5-10
 
 FUNCTION   ADDRESS
 ------------------
 main       3

 ADDR INSTRUCTION    OPERAND
 ------------------------------------
    0 GetGlobal      "main"
    1 Call           [0]
    2 Exit
    3 Alloca         [1]
    4 PushString     "hello"
    5 SetLocal       [0]
    6 PopOperand
    7 PushString     "world"
    8 SetLocal       [0]
    9 PopOperand
   10 GetLocal       [0]
   11 Print          [1]
   12 Return
 
 [ 피연산자 스택 ] | [ 함수 공간 ]
 1. [] | ["hello"]
 2. ["world"] | ["hello"]
 3. ["world"] | ["world"]
 4. [] | ["world"]
 5. ["world"] | ["world"]
 6. [] | ["world"]
 */
//string sourceCode = R""""(
//        function main() {
//            var local = 'hello';
//            local = 'world';
//            print local;
//        }
//    )"""";

/*
 출력 결과 5-11
 
 FUNCTION   ADDRESS
 ------------------
 main       3

 ADDR INSTRUCTION    OPERAND
 ------------------------------------
    0 GetGlobal      "main"
    1 Call           [0]
    2 Exit
    3 Alloca         [0]        -> 전역 변수는 함수 공간을 사용하지 않으므로 0이다.
    4 PushString     "world"
    5 SetGlobal      "global"
    6 PopOperand
    7 GetGlobal      "global"
    8 Print          [1]
    9 Return
 */
//string sourceCode = R""""(
//        function main() {
//            global = 'world';
//            print global;
//        }
//    )"""";

/*
 출력 결과 5-12
 
 FUNCTION   ADDRESS
 ------------------
 main       3

 ADDR INSTRUCTION    OPERAND
 ------------------------------------
    0 GetGlobal      "main"
    1 Call           [0]
    2 Exit
    3 Alloca         [1]
    4 PushNumber     0      -> 초기화식
    5 SetLocal       [0]
    6 PopOperand
    7 GetLocal       [0]    -> 조건식
    8 PushNumber     3
    9 LessThan
   10 ConditionJump  [19]   -> 조건식이 false이면 19번 주소로 점프
   11 GetLocal       [0]    -> 본문
   12 Print          [1]
   13 GetLocal       [0]    -> 증감식
   14 PushNumber     1
   15 Add
   16 SetLocal       [0]
   17 PopOperand
   18 Jump           [7]    -> 조건식이 true이면 7번 주소로 점프
   19 Return
 */
//string sourceCode = R""""(
//        function main() {
//            for i=0, i<3, i=i+1 {
//                print i;
//            }
//        }
//    )"""";

/*
 출력 결과 5-13
 
 FUNCTION   ADDRESS
 ------------------
 main       3

 ADDR INSTRUCTION    OPERAND
 ------------------------------------
    0 GetGlobal      "main"
    1 Call           [0]
    2 Exit
    3 Alloca         [0]
    4 PushBoolean    true
    5 ConditionJump  [9]    -> if절의 조건식 false면 9번 주소로 점프
    6 PushString     "if"
    7 Print          [1]
    8 Jump           [16]   -> if절의 조건식 true이면 16번 주소로 점프
    9 PushBoolean    true
   10 ConditionJump  [14]   -> elif절의 조건식이 false면 14번 주소로 점프
   11 PushString     "elif"
   12 Print          [1]
   13 Jump           [16]   -> elif절의 조건식 true이면 16번 주소로 점프
   14 PushString     "else"
   15 Print          [1]
   16 Return
 */
//string sourceCode = R""""(
//        function main() {
//            if true {
//                print 'if';
//            } elif true {
//                print 'elif';
//            } else {
//                print 'else';
//            }
//        }
//    )"""";

/*
 출력 결과 5-14
 
 FUNCTION   ADDRESS
 ------------------
 main       3

 ADDR INSTRUCTION    OPERAND
 ------------------------------------
    0 GetGlobal      "main"
    1 Call           [0]
    2 Exit
    3 Alloca         [1]
    4 PushNumber     0
    5 SetLocal       [0]
    6 PopOperand
    7 GetLocal       [0]
    8 PushNumber     3
    9 LessThan
   10 ConditionJump  [20]
   11 Jump           [14]   -> continue 문
   12 GetLocal       [0]    -> print 문
   13 Print          [1]
   14 GetLocal       [0]    -> 증감식
   15 PushNumber     1
   16 Add
   17 SetLocal       [0]
   18 PopOperand
   19 Jump           [7]
   20 Return
 */
//string sourceCode = R""""(
//        function main() {
//            for i=0, i<3, i=i+1 {
//                continue;
//                print i;
//            }
//        }
//    )"""";

/*
 출력 결과 5-15
 
 ; 토큰이 필요합니다. -> add함수의 return 제거
 FUNCTION   ADDRESS
 ------------------
 add        10
 main       3

 ADDR INSTRUCTION    OPERAND
 ------------------------------------
    0 GetGlobal      "main"
    1 Call           [0]
    2 Exit
    3 Alloca         [0]    -> main() 함수
    4 PushNumber     4
    5 PushNumber     3
    6 GetGlobal      "add"
    7 Call           [2]
    8 Print          [1]
    9 Return                -> 2번 주소로 점프
   10 Alloca         [2]    -> add() 함수
   11 GetLocal       [0]
   12 GetLocal       [1]
   13 Add
   14 PopOperand
   15 Return                -> 8번 주소로 점프
 
 [ 피연산자 스택 ] | [ 함수 공간 ]
 1. [4, 3] | [,]
 2. [4, 3, 10] | [,]
 3. [,] | [3, 4]
 4. [3, 4] | [3, 4]
 5. [7,] | [3, 4]
 6. [7] | []
 */
string sourceCode = R""""(
        function main() {
            print add(3, 4);
        }

        function add(a, b) {
            a + b;
        }
    )"""";

/*
 출력 결과 5-16
 
 FUNCTION   ADDRESS
 ------------------
 main       3

 ADDR INSTRUCTION    OPERAND
 ------------------------------------
    0 GetGlobal      "main"
    1 Call           [0]
    2 Exit
    3 Alloca         [0]
    4 PushString     "element"
    5 PushNumber     2
    6 PushNumber     3
    7 Add
    8 PushNumber     1
    9 PushArray      [3]
   10 PopOperand
   11 Return
 
 [ 피연산자 스택 ]
 1. ["element", 2, 3]
 2. ["element", 5]
 3. ["element", 5, 1]
 4. [[1, 5, "elemennt"]]
 */
//string sourceCode = R""""(
//        function main() {
//            [1, 2 + 3, 'element'];
//        }
//    )"""";

/*
 출력 결과 5-17
 
 FUNCTION   ADDRESS
 ------------------
 main       3

 ADDR INSTRUCTION    OPERAND
 ------------------------------------
    0 GetGlobal      "main"
    1 Call           [0]
    2 Exit
    3 Alloca         [0]
    4 PushNumber     2
    5 PushNumber     1
    6 PushArray      [2]
    7 PushNumber     0
    8 GetElement
    9 PopOperand
   10 Return
 
 [ 피연산자 스택 ]
 1. [[1,2]]
 2. [[1,2], 0]
 3. [1]
 */
//string sourceCode = R""""(
//        function main() {
//            [1, 2][0];
//        }
//    )"""";

/*
 출력 결과 5-18
 
 FUNCTION   ADDRESS
 ------------------
 main       3

 ADDR INSTRUCTION    OPERAND
 ------------------------------------
    0 GetGlobal      "main"
    1 Call           [0]
    2 Exit
    3 Alloca         [1]
    4 PushNumber     2
    5 PushNumber     1
    6 PushArray      [2]
    7 SetLocal       [0]
    8 PopOperand
    9 PushString     "element"
   10 GetLocal       [0]
   11 PushNumber     0
   12 SetElement
   13 PopOperand
   14 Return
 
 [ 피연산자 스택 ] | [ 함수 공간 ]
 1. [] | [[1, 2]]
 2. ["element"] | [[1, 2]]
 3. ["element", [1, 2]] | [[1, 2]]
 4. ["element", [1, 2], 0] | [[1, 2]]
 5. ["element"] | [["element", 2]]
 */
//string sourceCode = R""""(
//        function main() {
//            var array = [1, 2];
//            array[0] = 'element';
//        }
//    )"""";

auto main()->int {
    auto tokenList = scan(sourceCode);
    auto syntaxTree = parse(tokenList);
    auto objectCode = generate(syntaxTree);
    
//    printTokenList(tokenList);    // 3장 구문 분석
//    interpret(syntaxTree);      // 4장 인터프리터
    printObjectCode(objectCode);    // 5장 코드 생성
    
    return 0;
}

auto printObjectCode(tuple<vector<Code>, map<string, size_t>> objectCode)->void {
    auto codeList = get<0>(objectCode);
    auto functionTable = get<1>(objectCode);
    cout << setw(11) << left << "FUNCTION" << "ADDRESS" << endl;
    cout << string(18, '-') << endl;
    for (auto& [key, value] : functionTable)
        cout << setw(11) << key << value << endl;
    cout << endl;
    cout << "ADDR" << " " << setw(15) << "INSTRUCTION" << "OPERAND" << endl;
    cout << string(36, '-') << endl;
    for (size_t i = 0; i < codeList.size(); i++)
        cout << setw(4) << right << i << " " << codeList[i] << endl;
}
