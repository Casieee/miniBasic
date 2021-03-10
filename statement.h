#ifndef STATEMENT_H
#define STATEMENT_H

#include <QString>
#include <QWidget>
#include <QList>
#include "exp.h"
#include "tokenize.h"
#include "parse.h"

enum StatementType {REM, LET, PRINT, INPUT, GOTO, IF, END };

class Statement
{
public:
    Statement() {}
    virtual StatementType type() = 0;
    virtual QStringList toString() = 0;

    virtual int goToLine() {return 0;}
    virtual int getResult() {return 0;}
    virtual bool ifFlag() {return false;}
    virtual QString Identifier() {return "";}
    virtual ~Statement(){}
};

class LetStatement : public Statement {
public:
    LetStatement(QList<token> exp, EvaluationContext &context);
    StatementType type();
    QStringList toString();

    ~LetStatement(){}

private:
    parse tool;
    Expression* expression;
};

class PrintStatement : public Statement {
public:
    PrintStatement(QList<token> exp, EvaluationContext &context);
    StatementType type();
    QStringList toString();
    int getResult();

    ~PrintStatement(){}

private:
    parse tool;
    int result;
    Expression* printExp;
};

class IfStatement : public Statement {
public:
    IfStatement(QList<token> exp1, QList<token> exp2, EvaluationContext &context);
    StatementType type();
    QStringList toString();
    bool ifFlag();

    ~IfStatement(){}

private:
    bool flag;
    parse tool;
    QStringList parseTree;
    QString expression;
    Expression* ifExp;
    Expression* thenExp;
};
#endif // STATEMENT_H
