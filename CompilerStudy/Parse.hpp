//
//  Parse.hpp
//  CompilerStudy
//
//  Created by Hanhyung Lee on 2023/12/18.
//

#ifndef Parse_hpp
#define Parse_hpp

#include "Kind.h"
#include "Node.hpp"

using namespace std;

auto parseFunction()->Function*;
auto skipCurrent()->void;
auto skipCurrent(Kind kind)->void;
auto skipCurrentIf(Kind token_kind)->bool;
auto parseBlock()->vector<Statement*>;
auto parseFor()->For*;
auto parseIf()->If*;
auto parseExpressionStatement()->ExpressionStatement*;
auto parsePrint()->Print*;
auto parseVariable()->Variable*;
auto parseReturn()->Return*;
auto parseExpression()->Expression*;
auto parseBreak()->Break*;
auto parseAssignment()->Expression*;
auto parseContinue()->Continue*;
auto parseOr()->Expression*;
auto parseAnd()->Expression*;
auto parseRelational()->Expression*;
auto parseArithmetic1()->Expression*;
auto parseArithmetic2()->Expression*;
auto parseUnary()->Expression*;
auto parseOperand()->Expression*;
auto parseNullLiteral()->Expression*;
auto parseBooleanLiteral()->Expression*;
auto parseNumberLiteral()->Expression*;
auto parseStringLiteral()->Expression*;
auto parseListLiteral()->Expression*;
auto parseMapLiteral()->Expression*;
auto parseIdentifier()->Expression*;
auto parseInnerExpression()->Expression*;
auto parsePostfix(Expression* sub)->Expression*;
auto parseCall(Expression* sub)->Expression*;
auto parseElement(Expression* sub)->Expression*;

#endif /* Parse_hpp */
