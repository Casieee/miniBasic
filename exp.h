#ifndef EXP_H
#define EXP_H

#include <QString>
#include <QMap>
#include "tokenize.h"

class EvaluationContext;

enum ExpressionType { CONSTANT, IDENTIFIER, COMPOUND };

class Expression {
public:
    Expression() {}
    virtual ~Expression(){}
    virtual int eval(EvaluationContext & context) = 0;
    virtual QStringList toString() = 0;
    virtual ExpressionType type() = 0;
    virtual QString getIdentifierName() {return "";}
    virtual QString getOperator() {return "";}
    virtual Expression *getLHS() {return nullptr;}
    virtual Expression *getRHS() {return nullptr;}
};

class ConstantExpression:
        public Expression {
public:
    ConstantExpression(int number);
    ~ConstantExpression();
    int eval(EvaluationContext &context);
    QStringList toString();
    ExpressionType type();

private:
    int value;
};

class IdentifierExp: public Expression {
public:
    IdentifierExp(QString name);
    ~IdentifierExp() {}
    int eval(EvaluationContext &context);
    QStringList toString();
    ExpressionType type();
    QString getIdentifierName();

private:
    QString name;
};

class CompoundExp: public Expression {
public:
    CompoundExp(token op, Expression *lhs, Expression *rhs);
    ~CompoundExp() {}
    int eval(EvaluationContext &context);
    QStringList toString();
    ExpressionType type();
    QString getOperator();
    Expression* getLHS();
    Expression* getRHS();

private:
    token op;
    Expression *lhs, *rhs;
};

class EvaluationContext {
public:
    void setValue(QString var, int value);
    int getValue(QString var);
    bool isDefined(QString var);
    void clear();
    void display();

private:
    QMap<QString, int> symbolTable;
};

#endif // EXP_H
