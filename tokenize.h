#ifndef TOKENIZE_H
#define TOKENIZE_H

#include <QString>
#include <QDebug>

typedef enum {ADD, SUB, MULTI, DIV, POWER, EQUAL, GREATER, LOWER, NEGATIVE,
                LEFTPARE, RIGHTPARE, NUMBER, IDEN} TokenType;

class token {
public:
    token():iden(""), type(NUMBER), number(-1) {}
    ~token() {}
    TokenType getType() {
        return type;
    }
    int getNumber() {
        return number;
    }
    QString toString() {
        switch (type) {
        case ADD: return "+";
        case SUB: case NEGATIVE: return "-";
        case MULTI: return "*";
        case DIV: return "/";
        case POWER: return "^";
        case EQUAL: return "=";
        case GREATER: return ">";
        case LOWER: return "<";
        case LEFTPARE: return "(";
        case RIGHTPARE: return ")";
        case NUMBER: return QString::number(number);
        case IDEN: return iden;
        }
    }

    QString iden;
    TokenType type;
    int number;
};

class tokenize
{
public:
    tokenize();
    QList<token> analyze(QStringList cmdParts);
};

#endif // TOKENIZE_H
