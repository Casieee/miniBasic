#include "tokenize.h"
#include <QList>

tokenize::tokenize() {}

QList<token> tokenize::analyze(QStringList cmdParts) {
    QList<token> result;
    for(QString part: cmdParts) {
        for(int i = 0; i < part.length(); i++) {
            token t;
            //是否是变量
            if((part[i] >= 'a' && part[i] <= 'z') || (part[i] >= 'A' && part[i] <= 'Z')) {
                QString iden;
                iden.push_back(part[i]);
                for(i++; i < part.length(); i++) {
                    if((part[i] >= 'a' && part[i] <= 'z') || (part[i] >= 'A' && part[i] <= 'Z') || part[i].isNumber()) {
                        iden.push_back(part[i]);
                    }
                    else break;
                }
                i--;
                t.type = IDEN;
                t.iden = iden;
                result.push_back(t);
                continue;
            }
            if(part[i].isNumber()) {
                QString number;
                number.push_back(part[i]);
                for(i++; i < part.length(); i++) {
                    if(part[i].isNumber()){
                        number.push_back(part[i]);
                    }
                    else
                        break;
                }
                i--;
                t.type = NUMBER;
                t.number = number.toInt();
                result.push_back(t);
                continue;
            }
            if(part[i] == '(') {
                t.type = LEFTPARE;
                result.push_back(t);
                continue;
            }
            if(part[i] == ')') {
                t.type = RIGHTPARE;
                result.push_back(t);
                continue;
            }
            if(part[i] == '+') {
                t.type = ADD;
                result.push_back(t);
                continue;
            }
            if(part[i] == '-') {
                if(i == 0 || part[i-1] == '(' || part[i-1] == '=') {
                    t.type = NEGATIVE;
                    result.push_back(t);
                    continue;
                }
                else {
                    t.type = SUB;
                    result.push_back(t);
                    continue;
                }
            }
            if(part[i] == '/') {
                t.type = DIV;
                result.push_back(t);
                continue;
            }
            if(part[i] == '*') {
                if(part[i+1] == '*') {
                    i++;
                    t.type = POWER;
                    result.push_back(t);
                    continue;
                }
                else {
                    t.type = MULTI;
                    result.push_back(t);
                    continue;
                }
            }
            if(part[i] == '=') {
                t.type = EQUAL;
                result.push_back(t);
                continue;
            }
            if(part[i] == '<') {
                t.type = LOWER;
                result.push_back(t);
                continue;
            }
            if(part[i] == '>') {
                t.type = GREATER;
                result.push_back(t);
                continue;
            }
            throw "ILLEGAL TOKEN";
        }
    }

    return result;
}
