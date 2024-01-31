//
//  Parse.cpp
//  CompilerStudy
//
//  Created by Hanhyung Lee on 2023/12/18.
//

#include <stdio.h>
#include <iostream>
#include <set>
#include "Parse.hpp"

static vector<Token>::iterator current;

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
auto parseFunction()->Function* {
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

auto skipCurrent()->void {
    current++;
}

// 토큰 같은지 유무를 체크하는 보조함수
auto skipCurrent(Kind kind)->void {
    if (current->kind != kind) {
        cout << toString(kind) + " 토큰이 필요합니다.";
        exit(1);
    }
    current++;
}

auto skipCurrentIf(Kind token_kind)->bool {
    if (current->kind != token_kind)
        return false;
    current++;
    return true;
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
            case Kind::For:
                result.push_back(parseFor());
                break;
            case Kind::If:
                result.push_back(parseIf());
                break;
            case Kind::Print:
            case Kind::PrintLine:
                result.push_back(parsePrint());
                break;
            case Kind::Return:
                result.push_back(parseReturn());
                break;
            case Kind::Break:
                result.push_back(parseBreak());
                break;
            case Kind::Continue:
                result.push_back(parseContinue());
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

auto parseFor()->For* {
    auto result = new For();
    skipCurrent(Kind::For);
    result->variable = new Variable();
    result->variable->name = current->string;
    skipCurrent(Kind::Identifier);
    skipCurrent(Kind::Assignment);
    result->variable->expression = parseExpression();
    if (result->variable->expression  == nullptr) {
        cout << "for문에 초기화식이 없습니다.";
        exit(1);
    }
    skipCurrent(Kind::Comma);
    result->condition = parseExpression();
    if (result->condition == nullptr) {
        cout << "for문에 조건식이 없습니다.";
        exit(1);
    }
    skipCurrent(Kind::Comma);
    result->expression = parseExpression();
    if (result->expression == nullptr) {
        cout << "for문에 증감식이 없습니다.";
        exit(1);
    }
    skipCurrent(Kind::LeftBrace);
    result->block = parseBlock();
    skipCurrent(Kind::RightBrace);
    return result;
}

auto parseIf()->If* {
    auto result = new If();
    skipCurrent(Kind::If);
    do {
        auto condition = parseExpression();
        if (condition == nullptr) {
            cout << "if문에 조건식이 없습니다.";
            exit(1);
        }
        result->conditions.push_back(condition);
        skipCurrent(Kind::LeftBrace);
        result->blocks.push_back(parseBlock());
        skipCurrent(Kind::RightBrace);
    } while (skipCurrentIf(Kind::Elif));
    if (skipCurrentIf(Kind::Else)) {
        skipCurrent(Kind::LeftBrace);
        result->elseBlock = parseBlock();
        skipCurrent(Kind::RightBrace);
    }
    return result;
}

auto parsePrint()->Print* {
    auto result = new Print();
    result->lineFeed = current->kind == Kind::PrintLine;
    skipCurrent();
    if (current->kind != Kind::Semicolon) {
        do result->arguments.push_back(parseExpression());
        while (skipCurrentIf(Kind::Comma));
    }
    skipCurrent(Kind::Semicolon);
    return result;
}

auto parseReturn()->Return* {
    auto result = new Return();
    skipCurrent(Kind::Return);
    result->expression = parseExpression();
    if (result->expression == nullptr) {
        cout << "return문에 식이 없습니다.";
        exit(1);
    }
    skipCurrent(Kind::Semicolon);
    return result;
}

auto parseBreak()->Break* {
    auto result = new Break();
    skipCurrent(Kind::Break);
    skipCurrent(Kind::Semicolon);
    return result;
}

auto parseContinue()->Continue* {
    auto result = new Continue();
    skipCurrent(Kind::Continue);
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
    if (auto getVariable = dynamic_cast<GetVariable*>(result)) {
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

auto parseAnd()->Expression* {
    auto result = parseRelational();
    
    while(skipCurrentIf(Kind::LogicalAnd)) {
        auto temp = new And();
        temp->lhs = result;
        temp->rhs = parseAnd();
        result = temp;
    }
    return result;
}

auto parseRelational()->Expression* {
    set<Kind> operators = {
        Kind::Equal,
        Kind::NotEqual,
        Kind::LessThan,
        Kind::GreaterThan,
        Kind::LessOrEqual,
        Kind::GreaterOrEqual,
    };
    // + - * / 사칙연산
    auto result = parseArithmetic1();
    while (operators.count(current->kind)) {
        auto temp = new Relational();
        temp->kind = current->kind;
        skipCurrent();
        temp->lhs = result;
        temp->rhs = parseArithmetic1();
        result = temp;
    }
    return result;
}

// + -
auto parseArithmetic1()->Expression* {
    set<Kind> operators = {
        Kind::Add,
        Kind::Subtract
    };
    auto result = parseArithmetic2();
    while (operators.count(current->kind)) {
        auto temp = new Arithmetic();
        temp->kind = current->kind;
        skipCurrent();
        temp->lhs = result;
        temp->rhs = parseArithmetic2();
        result = temp;
    }
    return result;
}

// * / %
auto parseArithmetic2()->Expression* {
    set<Kind> operators = {
        Kind::Multiply,
        Kind::Divide,
        Kind::Modulo
    };
    auto result = parseUnary();
    while (operators.count(current->kind)) {
        auto temp = new Arithmetic();
        temp->kind = current->kind;
        skipCurrent();
        temp->lhs = result;
        temp->rhs = parseUnary();
        result = temp;
    }
    return result;
}

auto parseUnary()->Expression* {
    set<Kind> operators = {
        Kind::Add,
        Kind::Subtract,
    };
    while (operators.count(current->kind)) {
        auto result = new Unary();
        result->kind = current->kind;
        skipCurrent();
        result->sub = parseUnary();
        return result;
    }
    return parseOperand();
}

auto parseOperand()->Expression* {
    Expression* result = nullptr;
    switch (current->kind) {
        case Kind::NullLiteral:
            result = parseNullLiteral();
            break;
        case Kind::TrueLiteral:
        case Kind::FalseLiteral:
            result = parseBooleanLiteral();
            break;
        case Kind::NumberLiteral:
            result = parseNumberLiteral();
            break;
        case Kind::StringLiteral:
            result = parseStringLiteral();
            break;
        case Kind::LeftBraket:
            result = parseListLiteral();
            break;
        case Kind::LeftBrace:
            result = parseMapLiteral();
            break;
        case Kind::Identifier:
            result = parseIdentifier();
            break;
        case Kind::LeftParen:
            result = parseInnerExpression();
            break;
        default:
            break;
    }
    return parsePostfix(result);
}

// null 리터럴
auto parseNullLiteral()->Expression* {
    skipCurrent(Kind::NullLiteral);
    auto result = new NullLiteral();
    return result;
}

// true false
auto parseBooleanLiteral()->Expression* {
    auto result = new BooleanLiteral();
    result->value = current->kind == Kind::TrueLiteral;
    skipCurrent();
    return result;
}

// 숫자
auto parseNumberLiteral()->Expression* {
    auto result = new NumberLiteral();
    result->value = stod(current->string);
    skipCurrent(Kind::NumberLiteral);
    return result;
}

// 문자열
auto parseStringLiteral()->Expression* {
  auto result = new StringLiteral();
  result->value = current->string;
  skipCurrent(Kind::StringLiteral);
  return result;
}

// 배열
auto parseListLiteral()->Expression* {
    auto result = new ArrayLiteral();
    skipCurrent(Kind::LeftBraket);
    if (current->kind != Kind::RightBraket) {
        do result->values.push_back(parseExpression());
        while (skipCurrentIf(Kind::Comma));
    }
    skipCurrent(Kind::RightBraket);
    return result;
}

// 맵
auto parseMapLiteral()->Expression* {
    auto result = new MapLiteral();
    skipCurrent(Kind::LeftBrace);
    if (current->kind != Kind::RightBrace) {
        do {
            auto name = current->string;
            skipCurrent(Kind::StringLiteral);
            skipCurrent(Kind::Colon);
            auto value = parseExpression();
            result->values[name] = value;
        } while (skipCurrentIf(Kind::Comma));
    }
    skipCurrent(Kind::RightBrace);
    return result;
}

auto parseIdentifier()->Expression* {
    auto result = new GetVariable();
    result->name = current->string;
    skipCurrent(Kind::Identifier);
    return result;
}

auto parseInnerExpression()->Expression* {
    skipCurrent(Kind::LeftParen);
    auto result = parseExpression();
    skipCurrent(Kind::RightParen);
    return result;
}

auto parsePostfix(Expression* sub)->Expression* {
    while (true) {
        switch (current->kind) {
            case Kind::LeftParen:
                sub = parseCall(sub);
                break;
            case Kind::LeftBraket:
                sub = parseElement(sub);
                break;
            default:
                return sub;
        }
    }
}

// 함수 호출
auto parseCall(Expression* sub)->Expression* {
    auto result = new Call();
    result->sub = sub;
    skipCurrent(Kind::LeftParen);
    if (current->kind != Kind::RightParen) {
        do result->arguments.push_back(parseExpression());
        while (skipCurrentIf(Kind::Comma));
    }
    skipCurrent(Kind::RightParen);
    return result;
}

auto parseElement(Expression* sub)->Expression* {
    auto result = new GetElement();
    result->sub = sub;
    skipCurrent(Kind::LeftBraket);
    result->index = parseExpression();
    skipCurrent(Kind::RightBraket);
    return result;
}
