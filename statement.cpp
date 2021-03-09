#include "statement.h"

//LetStatement
LetStatement::LetStatement(QList<token> exp, EvaluationContext &context) {
    expression = tool.analyzeResult(exp);
    expression->eval(context);
}

StatementType LetStatement::type() {
    return LET;
}

//PrintStatement
PrintStatement::PrintStatement(QList<token> exp, EvaluationContext &context) {
    printExp = tool.analyzeResult(exp);
    result = printExp->eval(context);
}

StatementType PrintStatement::type() {
    return PRINT;
}

int PrintStatement::getResult() {
    return result;
}

//IfStatement
IfStatement::IfStatement(QList<token> exp1, QList<token> exp2, EvaluationContext &context): flag(false) {
    QList<token> exp11, exp12 = exp1;
    TokenType opType;
    for(auto word: exp1) {
        if(word.getType() == EQUAL || word.getType() == GREATER || word.getType() == LOWER) {
            exp12.removeFirst();
            opType = word.getType();
            break;
        }
        else {
            exp11.append(word);
            exp12.removeFirst();
        }
    }
    Expression* ifLeftExp = tool.analyzeResult(exp11);
    Expression* ifRightExp = tool.analyzeResult(exp12);
    switch (opType) {
    case EQUAL:
        if(ifLeftExp->eval(context) == ifRightExp->eval(context)) {
            flag = true;
        }
        break;
    case GREATER:
        if(ifLeftExp->eval(context) > ifRightExp->eval(context)) {
            flag = true;
        }
        break;
    case LOWER:
        if(ifLeftExp->eval(context) < ifRightExp->eval(context)) {
            flag = true;
        }
        break;
    default: break;
    }

    if(exp2.length() > 1) {
        thenExp = tool.analyzeResult(exp2);
        if(flag)
            thenExp->eval(context);
    }
}

StatementType IfStatement::type() {
    return IF;
}

bool IfStatement::ifFlag() {
    return flag;
}
