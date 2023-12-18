//
//  Parse.cpp
//  CompilerStudy
//
//  Created by Hanhyung Lee on 2023/12/18.
//

#include "Parse.hpp"

auto parse(vector<Token> tokens) -> Program* {
    auto result = new Program();                // Program init
    current = tokens.begin();                   // 첫번째 요소 가리킴
    while (current->kind != Kind::EndOfToken) { // EndOfToken 이 아닐때 까지 순회
        switch (current->kind) {
            case Kind::Function:
                result->functions.push_back(parseFunction());
                break;
            default:
                cout << *current << " 잘못된 구문입니다.";
                exit(1);
        }
    }
    return result;  // result 반환
}

// 함수의 정의를 분석
auto parseFunction() -> Function* {
    // 함수 시작
    auto result = new Function();
    skipCurrent(Kind::Function);
    
    // 함수 이름
    result->name = current->string;
    skipCurrent(Kind::Identifier);
    
    // 함수 괄호 오픈
    skipCurrent(Kind::LeftParen);
    if (current->kind != Kind::RightParen) {
        do {
            result->parameters.push_back(current->string);
            skipCurrent(Kind::Identifier);
        } while (skipCurrentIf(Kind::Comma));
    }
    skipCurrent(Kind::RightParen);
    
    // 함수 본문 { ... }
    skipCurrent(Kind::LeftBrace);
    result->block = parseBlock();
    skipCurrent(Kind::RightBrace);
    return result;
}

// 토큰 같은지 유무를 체크하는 보조함수
auto skipCurrent(Kind kind)->void {
    if (current->kind != kind) {
        cout << toString(kind) + " 토큰이 필요합니다.";
        exit(1);
    }
    current++;
}

// 함수 본문 분석
auto parseBlock()->vector<Statement*> {
    vector<Statement*> result;
    while(current->kind != Kind::RightBrace) {  // } 오른쪽 중괄호가 아닐때까지 순회
        switch (current->kind) {
            case Kind::Variable:
                // 변수의 선언
                result.push_back(parseVariable());
                break;
            case Kind::EndOfToken:
                cout << *current << " 잘못된 구문입니다.";
            default:
                // 현재 토큰이 특정 키워드들이 아니면 식의 시작으로 간주
                result.push_back(parseExpressionStatement());
                break;
        }
    }
    return result;
};

// 변수의 선언 구문
auto parseVariable()->Variable* {
    auto result = new Variable();
    // var 선언
    skipCurrent(Kind::Variable);
    
    // 변수 이름 식별자
    result->name = current->string;
    skipCurrent(Kind::Identifier);
    
    // 대입 연산자
    skipCurrent(Kind::Assignment);
    // 초기화 식 분석
    result->expression = parseExpression();
    
    // 세미콜론 확인
    skipCurrent(Kind::Semicolon);
    return result;
}

// 본문의 식을 분석
auto parseExpressionStatement()->ExpressionStatement* {
    auto result = new ExpressionStatement();
    result->expression = parseExpression();
    skipCurrent(Kind::Semicolon);
    return result;
}

// 식의 우선순위를 분석
auto parseExpression()->Expression* {
    // 우선순위가 가장 낮은 대입 연산자부터 분석을 시작
    return parseAssignment();
}

// 대입연산자
auto parseAssignment()->Expression* {
    // 대입연산자 보다 우선순위가 한 단계 높은 or 연산자
    auto result = parseOr();
    if (current->kind != Kind::Assignment)
        return result;
    skipCurrent(Kind::Assignment);
    
    // 변수값의 참조를 표한하는 GetVariable 노드인지 확인
    if (auto getVariable = dynamic_cast<GetVariable *>(result)) {
        // 재귀
        auto result = new SetVariable();
        result->name = getVariable->name;
        result->value = parseAssignment();
        return result;
    }
    
    if (auto getElement = dynamic_cast<GetElement*>(result)) {
        auto result = new SetElement();
        result->sub = getElement->sub;
        result->index = getElement->index;
        result->value = parseAssignment();
        return result;
    }
    
    cout << "잘못된 대입 연산 식입니다.";
    exit(1);
}

// or 연산자
auto parseOr()->Expression* {
    // or 보다 한 단계 높은 and 연산자
    auto result = parseAnd();
    
    // or 일째까지 반복
    while (skipCurrentIf(Kind::LogicalOr)) {
        auto temp = new Or();
        temp->lhs = result;
        temp->rhs = parseAnd();
        result = temp;
    }
    return result;
}
