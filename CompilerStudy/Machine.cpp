//
//  Machine.cpp
//  CompilerStudy
//
//  Created by Hanhyung Lee on 2024/02/26.
//

#include <stdio.h>
#include "Machine.hpp"

struct StackFrame {
    vector<any> variables;
    vector<any> operandStack;
    size_t instructionPointer = 0;
};

static vector<StackFrame> callStack;

static list<list<map<string, any>>> local;
static map<string, any> global;
static map<string, Function*> functionTable;
static list<Object*> objects;
extern map<string, function<any(vector<any>)>> builtinFunctionTable;

auto popOperand()->any;
auto pushOperand(any value)->void;
auto peekOperand()->any;
auto markObject(any value)->void;
auto collectGarbage()->void;
auto sweepObject()->void;

auto execute(tuple<vector<Code>, map<string, size_t>> objectCode)->void {
    // startup() 함수의 스택 프레임을 생성해 콜 스택에 넣는다.
    callStack.emplace_back();
    
    // 새로 생성된 스택 프레임의 명령어 포인터는 0이다.
    auto codeList = get<0>(objectCode);
    auto functionTable = get<1>(objectCode);
    while (true) {
        // 콜 스택의 최상단에 있는 스택 프레임의 명령어 포인터에서 실행할 코드를 가져온다.
        auto code = codeList[callStack.back().instructionPointer];
        
        switch (code.instruction) {
            case Instruction::Exit: {
                // startup() 함수의 스택 프레임을 콜 스택에서 제거하고 실행을 종료한다.
                callStack.pop_back();
                return;
            }
            case Instruction::GetGlobal: {
                // 주소 0의 GetGlobal "main" 코드가 실행된다.
                auto name = toString(code.operand);
                // 함수명으로 함수 테이블을 검색해 함수의 주소를 피연산자 스택에 넣는다.
                if (functionTable.count(name))
                    pushOperand(functionTable[name]);
                // 인자로 가진 이름으로 등록된 내장 함수가 있다면 피연산자 스택에 넣는다.
                else if (builtinFunctionTable.count(name))
                    pushOperand(builtinFunctionTable[name]);
                else if (global.count(name))
                    pushOperand(global[name]);
                else
                    pushOperand(nullptr);
                break;
            }
            case Instruction::Call: {
                // 호출할 함수의 주소는 피연산자 스택에 있으므로 피연산자 스택에서 값을 하나 꺼내온다.
                auto operand = popOperand();
                
                // 주소하면 스택 프레임을 생성하고 생성한 스택 프레임의 명령어 포인터를 호출할 함수의 주소로 설정한다.
                if (isSize(operand)) {
                    StackFrame stackFrame;
                    stackFrame.instructionPointer = toSize(operand);
                    
                    // 함수 호출 인자를 매개변수로 만든다.
                    for (auto i = 0; i < toSize(code.operand); i++) {
                        stackFrame.variables.push_back(callStack.back().operandStack.back());
                        callStack.back().operandStack.pop_back();
                    }
                    // 초기화된 스택 프레임을 콜 스택에 넣고 명령어 포인터가 증가되지 않도록 continue문으로 Call 명령의 실행을 끝낸다.
                    callStack.push_back(stackFrame);
                    continue;
                }
                if (isBuiltinFunction(operand)) {
                    // 내장 함수에 인자를 전달하고자 피연산자 스택에서 값을 꺼내 리스트로 만든다.
                    vector<any> arguments;
                    for (auto i = 0; i < toSize(code.operand); i++)
                        arguments.push_back(popOperand());
                    // 리스트를 인자로 내장 함수를 호출하고 반환값을 피연산자 스택에 넣는다.
                    pushOperand(toBuiltinFunction(arguments));
                    break;
                }
                pushOperand(nullptr);
                break;
            }
            case Instruction::Alloca: {
                // 지역변수들의 값을 저장할 공간을 확보한다.
                auto extraSize = toSize(code.operand);
                // Alloca 명령의 인자값만큼 변수 배열의 크기를 늘려 지역변수들의 값을 저장할 공간을 추가로 확보한다.
                auto currentSize = callStack.back().variables.size();
                callStack.back().variables.resize(currentSize + extraSize);
                break;
            }
            case Instruction::PushString: {
                pushOperand(code.operand);
                break;
            }
            case Instruction::PushNull: {
                // 책에 없는 부분 추가
                pushOperand(nullptr);
                break;
            }
            case Instruction::PushBoolean: {
                // 책에 없는 부분 추가
                pushOperand(code.operand);
                break;
            }
            case Instruction::PushNumber: {
                // 책에 없는 부분 추가
                pushOperand(code.operand);
                break;
            }
            case Instruction::Print: {
                for (auto i = 0; i < toSize(code.operand); i++) {
                    auto value = popOperand();
                    cout << value;
                }
                break;
            }
            case Instruction::PrintLine: {
                // 책에 없는 부분 추가
                cout << endl;
                break;
            }
            case Instruction::Return: {
                any result = nullptr;
                if (callStack.back().operandStack.empty() == false)
                    result = callStack.back().operandStack.back();
                // 현재 스택 프레임을 콜 스택에서 제거해 함수를 종료한다. 이제 현재 스택 프레임은 종료된 함수를 호출했던 함수의 스택 프레임이 된다.
                callStack.pop_back();
                // 임시로 보관했던 반환값을 현재 스택 프레임의 피연산자 스택에 넣는다.
                callStack.back().operandStack.push_back(result);
                collectGarbage();
                break;
            }
            case Instruction::Add: {
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNumber(lValue) && isNumber(rValue))
                    pushOperand(toNumber(lValue) + toNumber(rValue));
                else if (isString(lValue) && isString(rValue))
                    pushOperand(toString(lValue) + toString(rValue));
                else
                    // 지원하지 않는 조합
                    pushOperand(0.0);
                break;
            }
            case Instruction::Subtract: {
                // 책에 없는 부분 추가
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNumber(lValue) && isNumber(rValue))
                    pushOperand(toNumber(lValue) - toNumber(rValue));
                else
                    pushOperand(0.0);
                break;
            }
            case Instruction::Multiply: {
                // 책에 없는 부분 추가
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNumber(lValue) && isNumber(rValue))
                    pushOperand(toNumber(lValue) * toNumber(rValue));
                else
                    pushOperand(0.0);
                break;
            }
            case Instruction::Divide: {
                // 책에 없는 부분 추가
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNumber(lValue) && isNumber(rValue) && toNumber(rValue) == 0)
                    pushOperand(0.0);
                else if (isNumber(lValue) && isNumber(rValue))
                    pushOperand(toNumber(lValue) / toNumber(rValue));
                else
                    pushOperand(0.0);
                break;
            }
            case Instruction::Modulo: {
                // 책에 없는 부분 추가
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNumber(lValue) && isNumber(rValue) && toNumber(rValue) == 0)
                    pushOperand(0.0);
                else if (isNumber(lValue) && isNumber(rValue))
                    pushOperand(fmod(toNumber(lValue), toNumber(rValue)));
                else
                    pushOperand(0.0);
                break;
            }
            case Instruction::Absolute: {
                // 책에 없는 부분 추가
                auto value = popOperand();
                if (isNumber(value))
                    pushOperand(abs(toNumber(value)));
                else
                    pushOperand(0.0);
                break;
            }
            case Instruction::LogicalOr: {
                auto value = popOperand();
                if (isTrue(value)) {
                    pushOperand(value);
                    // 오른쪽 식의 코드를 실행하지 않도록 명령어 포인터를 LogicalOr 명령의 인자의 주소로 설정한다. continue문을 사용.
                    callStack.back().instructionPointer = toSize(code.operand);
                    continue;
                }
                break;
            }
            case Instruction::LogicalAnd: {
                // 책에 없는 부분 추가
                auto value = popOperand();
                if (isFalse(value)) {
                    pushOperand(value);
                    // 오른쪽 식의 코드를 실행하지 않도록 명령어 포인터를 LogicalOr 명령의 인자의 주소로 설정한다. continue문을 사용.
                    callStack.back().instructionPointer = toSize(code.operand);
                    continue;
                }
                break;
            }
            case Instruction::Equal: {
                // 책에 없는 부분 추가
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNull(lValue) && isNull(rValue))
                    pushOperand(true);
                else if (isBoolean(lValue) && isBoolean(rValue))
                    pushOperand(toBoolean(lValue) == toBoolean(rValue));
                else if (isNumber(lValue) && isNumber(rValue))
                    pushOperand(toNumber(lValue) == toNumber(rValue));
                else if (isString(lValue) && isString(rValue))
                    pushOperand(toString(lValue) == toString(rValue));
                else
                    pushOperand(false);
                break;
            }
            case Instruction::NotEqual: {
                // 책에 없는 부분 추가
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNull(lValue) && isNull(rValue))
                    pushOperand(false);
                else if (isNull(lValue) || isNull(rValue))
                    pushOperand(true);
                if (isBoolean(lValue) && isBoolean(rValue))
                    pushOperand(toBoolean(lValue) != toBoolean(rValue));
                else if (isNumber(lValue) && isNumber(rValue))
                    pushOperand(toNumber(lValue) != toNumber(rValue));
                else if (isString(lValue) && isString(rValue))
                    pushOperand(toString(lValue) != toString(rValue));
                else
                    pushOperand(false);
                break;
            }
            case Instruction::LessThan: {
                // 이게 없으면 크래시 난다.
                // 책에 없는 부분 추가
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNumber(lValue) && isNumber(rValue))
                    pushOperand(toNumber(lValue) < toNumber(rValue));
                else
                    pushOperand(false);
                break;
            }
            case Instruction::GreaterThan: {
                // 책에 없는 부분 추가
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNumber(lValue) && isNumber(rValue))
                    pushOperand(toNumber(lValue) > toNumber(rValue));
                else
                    pushOperand(false);
                break;
            }
            case Instruction::LessOrEqual: {
                // 책에 없는 부분 추가
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNumber(lValue) && isNumber(rValue))
                    pushOperand(toNumber(lValue) <= toNumber(rValue));
                else
                    pushOperand(false);
                break;
            }
            case Instruction::GreaterOrEqual: {
                // 책에 없는 부분 추가
                auto rValue = popOperand();
                auto lValue = popOperand();
                if (isNumber(lValue) && isNumber(rValue))
                    pushOperand(toNumber(lValue) >= toNumber(rValue));
                else
                    pushOperand(false);
                break;
            }
            case Instruction::GetLocal: {
                // 현재 스택 프레임의 변수 배열에서 오프셋 위치에 저장돼 있는 값을 피연산자 스택에 넣는다.
                auto index = toSize(code.operand);
                pushOperand(callStack.back().variables[index]);
                break;
            }
            case Instruction::SetLocal: {
                // 오프셋을 인자로 가지므로 현재 스택 프레임의 변수 배열의 오프셋 위치에 값을 대입한다.
                auto index = toSize(code.operand);
                callStack.back().variables[index] = peekOperand();
            }
            case Instruction::SetGlobal: {
                // 명령의 인자인 전역변수의 이름과 피연산자 스택에서 꺼낸 값을 global 전역변수에 키와 값으로 등록한다.
                auto name = toString(code.operand);
                global[name] = peekOperand();
                break;
            }
            case Instruction::Jump: {
                // 현재 스택 프레임의 명령어 포인터를 인자의 주소로 변경한다. continue문을 사용했음에 유의하자.
                callStack.back().instructionPointer = toSize(code.operand);
                continue;
            }
            case Instruction::ConditionJump: {
                auto condition = popOperand();
                // 조건식의 결과값이 참이라면 점프를 하지 않고 그대로 진행해 if문이나 for문의 본문이 실행하도록 한다.
                if (isTrue(condition))
                    break;
                // 조건식의 결과값이 참이 아니라면 인자의 주소로 변경하여 본문의 끝으로 점프
                callStack.back().instructionPointer = toSize(code.operand);
                continue;
            }
            case Instruction::PushArray: {
                auto result = new Array();
                // 인자는 원소의 개수이므로 피연산자 스택에서 원소의 개수만큼 꺼내 생성해둔 배열에 추가한다.
                auto size = toSize(code.operand);
                for (auto i = size; i > 0; i--)
                    result->values.push_back(popOperand());
                // 원소들이 담긴 배열을 피연산자 스택에 넣는다.
                pushOperand(result);
                objects.push_back(result);
                break;
            }
            case Instruction::PushMap: {
                // 책에 없는 부분 추가
                auto result = new Map();
                for (size_t i = 0; i < toSize(code.operand); i++) {
                    auto value = popOperand();
                    auto key = toString(popOperand());
                    result->values[key] = value;
                }
                pushOperand(result);
                objects.push_back(result);
                break;
            }
            case Instruction::ReverseSign: {
                // 책에 없는 부분 추가
                auto value = popOperand();
                if (isNumber(value))
                    pushOperand(toNumber(value) * -1);
                else
                    pushOperand(0.0);
                break;
            }
            case Instruction::GetElement: {
                auto index = popOperand();
                auto sub = popOperand();    // object -> sub
                // 피연산자 식의 값이 배열이고 인덱스 식의 값이 숫자인 경우에는 배열의 원소값을 참조해 피연산자 스택에 넣는다.
                if (isArray(sub) && isNumber(index))
                    pushOperand(getValueOfArray(sub, index));
                // 피연산자 식의 값이 맵이고 인덱스 식의 값이 문자열인 경우에는 맵의 원소값을 참조해 피연산자 스택에 넣는다.
                else if (isMap(sub) && isString(index))
                    pushOperand(getValueOfMap(sub, index));
                else
                    pushOperand(nullptr);
                break;
            }
            case Instruction::SetElement: {
                // 피연산자 스택에서 인덱스 식과 피연산자 식의 값을 차례로 꺼내온다.
                auto inndex = popOperand();
                auto sub = popOperand();
                // 피연산자 스택에 연산의 결과값이 남아야 하므로 peekOperand() 함수를 사용했음에 유의
                if (isArray(sub) && isNumber(index))
                    setValueOfArray(sub, index, peekOperand());
                // 피연산자 식의 값이 맵이고 인덱스 식의 값이 문자열인 경우에는 맵의 원소값을 피연산자 스택에 있는 값으로 변경한다.
                else if (isMap(sub) && isString(index))
                    setValueOfMap(sub, index, peekOperand());
                break;
            }
            case Instruction::PopOperand: {
                // 책에 없는 부분 추가
                popOperand();
                break;
            }
        }
        // 명령어 포인터를 1증가 시킨다.
        callStack.back().instructionPointer += 1;
    }
}

// 현재 스택 프레임의 피연산자 스택에서 값을 꺼내 반환한다.
auto popOperand()->any {
    auto value = callStack.back().operandStack.back();
    callStack.back().operandStack.pop_back();
    return value;
}

// 현재 스택 프레임의 피연산자 스택에 값을 넣는다.
auto pushOperand(any value)->void {
    callStack.back().operandStack.push_back(value);
}

// 스택에서 값을 제거하지 않는다.
auto peekOperand()->any {
    return callStack.back().operandStack.back();
}

// 마크 스윕을 구현하기 위한 객체에 참조 표시를 하는 함수
auto markObject(any value)->void {
    if (isArray(value)) {
        if (toArray(value)->isMarked) return;
        toArray(value)->isMarked = true;
        
        for (auto& value: toArray(value)->values)
            markObject(value);
    }
}

// 변수들을 순회하는 함수
auto collectGarbage()->void {
    for (auto& stackFrame: callStack) {
        for (auto& value: stackFrame.variables)
            markObject(value);
        for (auto& value: stackFrame.operandStack)
            markObject(value);
        for (auto& [key, value]: global)
            markObject(value);
        for (auto& [key, value]: global)
            markObject(value);
        sweepObject();
    }
}

// 스윕은 단순히 objects 리스트를 순회하면서 참조 표시가 된 객체는 다음 번의 마크 단계를 위해 표시를 제거하고 표시가 돼 있지 않은 객체는 메모리를 해제한 후 objects 리스트에서 제거한다.
auto sweepObject()->void {
    objects.remove_if([](Object* object) {
        if (object->isMarked) {
            object->isMarked = false;
            return false;
        }
        delete object;
        return true;
    });
}
