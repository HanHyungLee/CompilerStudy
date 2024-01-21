//
//  Instruction.hpp
//  CompilerStudy
//
//  Created by Hanhyung Lee on 2024/01/19.
//

#ifndef Instruction_hpp
#define Instruction_hpp

#include <stdio.h>

enum class Instruction {
    Exit,               // 프로그램 종료
    Call,               // 함수 호출
    Alloca,             // 메모리 할당
    Return,             // 함수 종료
    Jump,               // 분기
    ConditionJump,      // 조건 분기
    Print, PrintLine,   // 콘솔 출력
    
    LogicalOr, LogicalAnd,          // 논리 연산
    
    Add, Subtract,                  // 산술 연산
    Multiply, Divide, Modulo,
    
    Equal, NotEqual,                // 비교 연산
    LessThan, GreaterThan,
    LessOrEqual, GreaterOrEqual,
    
    Absolute, ReverseSign,          // 단항 연산
    
    GetElement, SetElement,         // 원소 연산
    GetGlobal, SetGlobal,           // 전역 변수 연산
    GetLocal, SetLocal,             // 지역 변수 연산
    
    PushNull, PushBoolean,          // 프리미티브 타입 리터럴 연산
    PushNumber, PushString,
    
    PushArray, PushMap,             // 레퍼런스 타입 리터럴 연산
    PopOperand,                     // 피연산자 스택 연산
};

#endif /* Instruction_hpp */
