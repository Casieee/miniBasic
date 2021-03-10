#include "exp.h"
#include <QDebug>

void EvaluationContext::setValue(QString var, int value) {
    symbolTable.insert(var, value);
}

int EvaluationContext::getValue(QString var) {
    return symbolTable.value(var);
}

bool EvaluationContext::isDefined(QString var) {
    return symbolTable.contains(var);
}

void EvaluationContext::clear() {
    symbolTable.clear();
}

void EvaluationContext::display() {
    for(QMap<QString, int>::iterator elem = symbolTable.begin(); elem != symbolTable.end(); elem++) {
        qDebug() << elem.key() << elem.value();
    }
}

ConstantExpression::ConstantExpression(int number): value(number) {
}

ConstantExpression::~ConstantExpression() {

}

int ConstantExpression::eval(EvaluationContext &context) {
    return value;
}

QStringList ConstantExpression::toString() {
    QStringList constant;
    constant.append(QString::number(value));
    return constant;
}

ExpressionType ConstantExpression::type() {
    return CONSTANT;
}

IdentifierExp::IdentifierExp(QString name): name(name) {}

int IdentifierExp::eval(EvaluationContext &context) {
    if(!context.isDefined(name))
       throw "UNDEFINED IDENTIFIER";
    return context.getValue(name);
}

QStringList IdentifierExp::toString() {
    QStringList iden;
    iden.append(name);
    return iden;
}

ExpressionType IdentifierExp::type() {
    return IDENTIFIER;
}

QString IdentifierExp::getIdentifierName() {
    return name;
}

CompoundExp::CompoundExp(token op, Expression *lhs, Expression *rhs):
    op(op),lhs(lhs),rhs(rhs){}

int CompoundExp::eval(EvaluationContext &context) {
    int right = rhs->eval(context);
    if(op.getType() == EQUAL) {
        context.setValue(lhs->getIdentifierName(), right);
        return right;
    }
    int left = lhs->eval(context);
    switch (op.getType()) {
    case ADD: return left + right;
    case SUB: case NEGATIVE: return left - right;
    case MULTI: return left * right;
    case DIV: {
        if(right == 0)
            throw "DIVIDEND CANNOT BE ZERO";
        return left / right;
    }
    case POWER:
    {
        int result = 1;
        for(int i = 0; i < right; i++) {
            result *= left;
        }
        return result;
    }
    default: break;
    }
    return 0;
}

ExpressionType CompoundExp::type() {
    return COMPOUND;
}

QStringList CompoundExp::toString() {
    QStringList str, lstr = lhs->toString(), rstr = rhs->toString();
    str.append(op.toString());
    for(int i = 0; i < lstr.length(); i++) {
        lstr[i].insert(0,'\t');
    }
    str.append(lstr);
    for(int i = 0; i < rstr.length(); i++) {
        rstr[i].insert(0,'\t');
    }
    str.append(rstr);
    return str;
}

QString CompoundExp::getOperator() {
    switch (op.getType()) {
    case ADD: return "+";
    case SUB: case NEGATIVE: return "-";
    case MULTI: return "*";
    case DIV: return "/";
    case POWER: return "**";
    default: throw "ILLEGAL OPERATOR";
    }
    return "";
}

Expression* CompoundExp::getLHS() {
    return lhs;
}

Expression* CompoundExp::getRHS() {
    return rhs;
}


