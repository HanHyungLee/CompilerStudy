//
//  Generator.cpp
//  CompilerStudy
//
//  Created by Hanhyung Lee on 2024/01/19.
//

#include "Generator.hpp"

// Code 객체 저장 목록
static vector<Code> codeList;

// 함수의 이름과 주소를 키와 값으로 보관
static map<string, size_t> functionTable;

// 리스트는 이중 연결 리스트로 선형 구조를 가지는 시퀀스 컨테이너이며 노드 기반으로 데이터를 저장한다.
// 동일한 함수를 여러번 호출할때 이름이 같은 변수들이 서로 다른 값을 가질 수 있도록 하려면 상대 주소를 배정해야 한다.
// map은 변수의 이름과 오프셋을 키와 값으로 가지고 있고, 리스트는 변수의 유효 범위를 관리한다.
static list<map<string, size_t>> symbolStack;

// 블럭 단위로 가장 큰 오프셋 값을 관리한다.
static vector<size_t> offsetStack;

// 오프셋 스택은 블럭이 끝나면 값을 버리므로 이 값을 보관할 전역 변수 선언
static size_t localSize;

// 가장 가까운 for문의 증감식의 주소로 점프하도록 Jump 코드의 주소를 담을 리스트를 전역변수로 선언
static vector<vector<size_t>> continueStack;

//
//static vector<vector<size_t>> breakStack;

auto writeCode(Instruction instruction)->size_t;
auto writeCode(Instruction instruction, any operand)->size_t;

auto setLocal(string name)->void;

auto generate(Program* program)->tuple<vector<Code>, map<string, size_t>> {
    // 함수의 주소를 가져온다.
    writeCode(Instruction::GetGlobal, string("main"));
    // 함수를 호출한다. 매개변수가 없으므로 0
    writeCode(Instruction::Call, static_cast<size_t>(0));
    // 함수가 종료되면 프로그램 종료
    writeCode(Instruction::Exit);
    // 함수 노드 리스트 순회
    for (auto& node: program->functions)
        node->generate();
    return {codeList, functionTable};
}

auto writeCode(Instruction instruction)->size_t {
    codeList.push_back({instruction});
    return codeList.size() - 1;
}

auto writeCode(Instruction instruction, any operand)->size_t {
    codeList.push_back({instruction, operand});
    return codeList.size() - 1;
}

// 단순히 코드의 인자를 코드 리스트의 크기로 설정한다.
auto patchAddress(size_t codeIndex)->void {
    codeList[codeIndex].operand = codeList.size();
}

// MARK: - Statement

// 코드의 인자를 매개변수로 받은 값으로 설정한다.
auto patchOperand(size_t codeIndex, size_t operand)->void {
    codeList[codeIndex].operand = operand;
}

// 함수의 크기를 관리하는 전역변수를 0으로 초기화한 후 오프셋 스택에 기본값인 0을 추가해 블럭을 생성하고, 심볼 스택에 빈 블럭을 생성한다.
auto initBlock()->void {
    localSize = 0;
    offsetStack.push_back(0);
    symbolStack.emplace_front();
}

// 오프셋 스택과 심볼 스택에 생성된 블럭을 제거한다.
auto popBlock()->void {
    offsetStack.pop_back();
    symbolStack.pop_front();
}

// 함수 목적 코드 생성
auto Function::generate()->void {
    // 현재 주소를 함수의 주소로 함수 테이블에 등록
    functionTable[name] = codeList.size();
    // 함수를 실행하기 위해 필요한 메모리 공간을 할당
    auto temp = writeCode(Instruction::Alloca);
    // 함수의 블럭을 초기화
    initBlock();
    // 매개변수의 이름 등록
    for (auto& name: parameters)
        setLocal(name);
    // 함수 내용 목적 코드 생성
    for (auto& node: block)
        node->generate();
    popBlock();
    // 본문 노드들의 순회가 끝나면 전역변수 localSize의 값으로 Alloca 명령의 인자를 패치한다.
    patchOperand(temp, localSize);
    // 함수 종료
    writeCode(Instruction::Return);
}

auto Print::generate()->void {
    // 인자 식 노드들을 순회 목적 코드 생성
    for (auto i = arguments.size(); i > 0; i--)
        arguments[i - 1]->generate();
    // 피연산자 스택에서 값을 꺼내 콘솔에 출력
    writeCode(Instruction::Print, arguments.size());
    // 개행 여부
    if (lineFeed)
        writeCode(Instruction::PrintLine);
}

// 식 노드를 순회해 반환값을 피연산자 스택에 넣는 코드를 생성하고 Return 명령을 생성
auto Return::generate()->void {
    expression->generate();
    writeCode(Instruction::Return);
}

// 변수를 심볼 스택의 현재 블럭에 등록한 후 오프셋 스택의 현재 블럭의 값을 1 증가시킨다. 또한 함수의 크기를 저장하는 전역변수 localSize의 값을 기존의 localSize의 값과 오프셋 스택의 현재 블럭 값 중 큰 값으로 설정해 함수를 실행하는데 필요한 공간의 크기를 갱신한다.
// 오프셋은 단순히 0부터 순차적 증가하는 값.
auto setLocal(string name)->void {
    symbolStack.front()[name] = offsetStack.back();
    offsetStack.back() += 1;
    localSize = max(localSize, offsetStack.back());
}

// 변수명을 매개변수로 받아 오프셋을 반환한다. 심볼 스택의 블럭들을 생성된 순서의 역순으로 순회하며 찾는다. 변수가 없다면 size_t 자료형이 표현할 수 있는 가장 큰 값을 반환한다.
auto getLocal(string name)->size_t {
    for (auto& symbolTable: symbolStack) {
        if (symbolTable.count(name))
            return symbolTable[name];
    }
    return SIZE_MAX;
}

// 변수의 선언
auto Variable::generate()->void {
    // 현재 블럭에 변수의 이름을 등록한다.
    setLocal(name);
    // 초기화식 노드 순회
    expression->generate();
    writeCode(Instruction::SetLocal, getLocal(name));
    // 스택 비움
    writeCode(Instruction::PopOperand);
}

// 심볼 스택에 빈 블럭을 생성한다. 그리고 오프셋 스택에 새 블럭을 생성한다.
auto pushBlock()->void {
    symbolStack.emplace_front();
    offsetStack.push_back(offsetStack.back());
}

auto For::generate()->void {
    continueStack.emplace_back();
    // 제어 변수를 초기화하는 목적 코드 생성. 제어 변수의 유효 범위 설정
    pushBlock();
    variable->generate();
    // 조건식의 목적 코드 생성
    auto jumpAddress = codeList.size();
    condition->generate();
    // 조건식의 결과가 참이 아니라면 점프한다.
    auto conditionJump = writeCode(Instruction::ConditionJump);
    // 조건식의 결과가 참이면 목적 코드 생성
    for (auto& node: block)
        node->generate();
    // 현재 주소를 임시로 보관
    auto continueAddress = codeList.size();
    // 증감식 실행. 식의 결과값을 버려지도록 PopOperand 명령 생성
    expression->generate();
    writeCode(Instruction::PopOperand);
    // 증감식 실행 후 반복문이 조건식부터 다시 실행되도록 조건식의 주소로 점프하는 코드 생성
    writeCode(Instruction::Jump, jumpAddress);
    // for문의 목적 코드를 모두 생성했으니 조건식의 결과가 참이 아닌 경우에는 현재 주소로 점프하도록 한다.
    patchAddress(conditionJump);
    // for문의 블럭 제거
    popBlock();
    //
    for (auto& jump: continueStack.back())
        patchOperand(jump, continueAddress);
    continueStack.pop_back();
}

auto Break::generate()->void {
    
}

auto Continue::generate()->void {
    if (continueStack.empty()) return;
    auto jumpCode = writeCode(Instruction::Jump);
    continueStack.back().push_back(jumpCode);
}

auto If::generate()->void {
    // if문은 하나의 본문을 실행했다면 if문의 끝으로 점프해야 한다. 목적 코드 작성 중에는 끝 주소를 알 수 없으므로 생성한 점프 명령 코드들을 보관할 리스트를 생성하고, 조건식의 개수만큼 반복
    vector<size_t> jumpList;
    for (auto i = 0; i < conditions.size(); i++) {
        // 조건식의 결과가 참이 아니라면 다음 조건식의 주소로 점프
        conditions[i]->generate();
        auto conditionJump = writeCode(Instruction::ConditionJump);
        // 조건식의 결과가 참이라면 for문과 동일하게 블럭을 생성하고, 본문의 문 노드들을 순회해 목적코드를 생성한 후 생성했던 블럭을 제거한다.
        pushBlock();
        for (auto& node: blocks[i])
            node->generate();
        popBlock();
        // 본문을 실행한 후 다른 elif절이나 else절을 실행하지 않도록 Jump명령을 생성
        jumpList.push_back(writeCode(Instruction::Jump));
        // 하나의 조건식에 대한 본문의 목적 코드 생성이 끝났으므로 조건식의 결과가 참이 아니라면 현재 주소로 점프
        patchAddress(conditionJump);
    }
    // else절이 있다면 else절 본문의 문 노드들을 순회해 목적 코드를 생성.
    if (elseBlock.empty() == false) {
        pushBlock();
        for (auto& node: elseBlock)
            node->generate();
        popBlock();
    }
    // if문의 목적 코드 생성이 끝났으므로 현재 주소가 if문의 끝 주소가 된다. 하나의 본문을 실행한 후에는 현재 주소로 점프하도록 점프 코드 리스트를 순회해 Jump명령을 패치한다.
    for (auto& jump: jumpList)
        patchAddress(jump);
}

// 식의 결과값
auto ExpressionStatement::generate()->void {
    expression->generate();
    // 피연산자 스택에서 값을 비운다.
    writeCode(Instruction::PopOperand);
}

// MARK: - Expression

auto Or::generate()->void {
    // 왼쪽 식 노드 순회해 코드를 생성
    lhs->generate();
    // 왼쪽 식의 값을 피연산자 스택에서 꺼낸다. LogicalOr 코드의 주소를 임시로 보관
    auto _or = writeCode(Instruction::LogicalOr);
    // 오른쪽 식 노드 순회해 코드를 생성
    rhs->generate();
    // LogicalOr 명령의 인자를 현재 주소로 패치한다.
    patchAddress(_or);
}

// 책에 빠진 내용
auto And::generate()->void {
    lhs->generate();
    auto logicalAnd = writeCode(Instruction::LogicalAnd);
    rhs->generate();
    patchAddress(logicalAnd);
}

// 비교문 (책에 빠진 내용, 출력 결과 5-12 전에 추가)
auto Relational::generate()->void {
    map<Kind, Instruction> instructions = {
        {Kind::Equal,          Instruction::Equal},
        {Kind::NotEqual,       Instruction::NotEqual},
        {Kind::LessThan,       Instruction::LessThan},
        {Kind::GreaterThan,    Instruction::GreaterThan},
        {Kind::LessOrEqual,    Instruction::LessOrEqual},
        {Kind::GreaterOrEqual, Instruction::GreaterOrEqual}
    };
    lhs->generate();
    rhs->generate();
    writeCode(instructions[kind]);
}

// 산술 연산자
auto Arithmetic::generate()->void {
    map<Kind, Instruction> instructions = {
        {Kind::Add, Instruction::Add},
        {Kind::Subtract, Instruction::Subtract},
        {Kind::Multiply, Instruction::Multiply},
        {Kind::Divide, Instruction::Divide},
        {Kind::Modulo, Instruction::Modulo},
    };
    lhs->generate();
    rhs->generate();
    writeCode(instructions[kind]);
}

auto Unary::generate()->void {
    
}

// 함수 호출
auto Call::generate()->void {
    // 인자값들의 순서를 유지하기 위해 역순으로 인자 식 노드들을 순회한다.
    for (auto i = arguments.size(); i > 0; i--)
        arguments[i - 1]->generate();
    // 피연산자식 노드를 순회해 목적 코드 생성
    sub->generate();
    // Call 명령 생성
    writeCode(Instruction::Call, arguments.size());
}

// 원소값 참조
auto GetElement::generate()->void {
    sub->generate();
    index->generate();
    writeCode(Instruction::GetElement);
}

// 원소값 수정
auto SetElement::generate()->void {
    value->generate();
    sub->generate();
    index->generate();
    writeCode(Instruction::SetElement);
}

// 변수 참조
auto GetVariable::generate()->void {
    if (getLocal(name) == SIZE_MAX)
        writeCode(Instruction::GetGlobal, name);
    else
        writeCode(Instruction::GetLocal, getLocal(name));
}

// 변수값 수정
auto SetVariable::generate()->void {
    value->generate();
    if (getLocal(name) == SIZE_MAX)
        writeCode(Instruction::SetGlobal, name);
    else
        writeCode(Instruction::SetLocal, getLocal(name));
}

// 책에 빠진 내용
auto NullLiteral::generate()->void {
    
}

auto BooleanLiteral::generate()->void {
    // 피연산자 스택에 Boolean 추가 (책에 빠진 내용, 출력 결과 5-8 전에 추가)
    writeCode(Instruction::PushBoolean, value);
}

auto NumberLiteral::generate()->void {
    // 피연산자 스택에 숫자 추가 (책에 빠진 내용, 출력 결과 5-5 전에 추가)
    writeCode(Instruction::PushNumber, value);
}

auto StringLiteral::generate()->void {
    // 피연산자 스택에 문자열 추가
    writeCode(Instruction::PushString, value);
}

auto ArrayLiteral::generate()->void {
    // 스택에 넣었다가 다시 순서대로 빼면 순서가 뒤바뀌므로 역순으로 순회
    for (auto i = values.size(); i > 0; i--)
        values[i - 1]->generate();
    // 피연산자 스택에 쌓여 있는 원소값들을 모두 꺼내 배열을 생성하는 PushArray 명령을 생성
    writeCode(Instruction::PushArray, values.size());
    
}

auto MapLiteral::generate()->void {
    
}
