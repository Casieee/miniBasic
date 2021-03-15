#include "parse.h"
#include <QStack>

parse::parse(){

}

Expression* parse::analyzeResult(QList<token> expression) {
    Expression* root, *tmp;
    QStack<token> opStack;
    QStack<Expression*> expStack;

    for(int i = 0; i < expression.length(); i++) {
        token pre = expression[i];
        switch (pre.getType()) {
        case NEGATIVE:
            opStack.push(pre);
            break;
        case ADD: case SUB:
        {
            while (!opStack.isEmpty()&& opStack.top().getType() != EQUAL && opStack.top().getType() != LEFTPARE) {
                token top = opStack.pop();
                if(top.getType() == NEGATIVE) {
                    Expression* operand = expStack.pop();
                    Expression* constant0 = new ConstantExpression(0);
                    tmp = new CompoundExp(top, constant0, operand);
                    expStack.push(tmp);
                }
                else {
                    Expression* operand2 = expStack.pop();
                    Expression* operand1 = expStack.pop();
                    tmp = new CompoundExp(top, operand1, operand2);
                    expStack.push(tmp);
                }
            }
            opStack.push(pre);
            break;
        }
        case MULTI: case DIV:
        {
            while (!opStack.isEmpty()&& (opStack.top().getType() == MULTI ||
                   opStack.top().getType() == DIV ||
                   opStack.top().getType() == POWER ))
            {
                token top = opStack.pop();
                if(top.getType() == NEGATIVE) {
                    Expression* operand = expStack.pop();
                    Expression* constant0 = new ConstantExpression(0);
                    tmp = new CompoundExp(top, constant0, operand);
                    expStack.push(tmp);
                }
                else {
                    Expression* operand2 = expStack.pop();
                    Expression* operand1 = expStack.pop();
                    tmp = new CompoundExp(top, operand1, operand2);
                    expStack.push(tmp);
                }
            }
            opStack.push(pre);
            break;
        }
        case POWER:
            opStack.push(pre);
            break;
        case EQUAL:
            while (!opStack.isEmpty()) {
                token top = opStack.pop();
                if(top.getType() == NEGATIVE) {
                    Expression* operand = expStack.pop();
                    Expression* constant0 = new ConstantExpression(0);
                    tmp = new CompoundExp(top, constant0, operand);
                    expStack.push(tmp);
                }
                else {
                    Expression* operand2 = expStack.pop();
                    Expression* operand1 = expStack.pop();
                    tmp = new CompoundExp(top, operand1, operand2);
                    expStack.push(tmp);
                }
            }
            opStack.push(pre);
            break;
        case LEFTPARE:
            opStack.push(pre);
            break;
        case RIGHTPARE:
        {
            token inPare = opStack.pop();
            while (inPare.getType() != LEFTPARE) {
                if(inPare.getType() == NEGATIVE) {
                    Expression* operand = expStack.pop();
                    Expression* constant0 = new ConstantExpression(0);
                    tmp = new CompoundExp(inPare, constant0, operand);
                    expStack.push(tmp);
                    inPare = opStack.pop();
                }
                else {
                    Expression* operand2 = expStack.pop();
                    Expression* operand1 = expStack.pop();
                    tmp = new CompoundExp(inPare, operand1, operand2);
                    expStack.push(tmp);
                    inPare = opStack.pop();
                }
            }
            break;
        }
        case NUMBER:
            tmp = new ConstantExpression(pre.getNumber());
            expStack.push(tmp);
            break;
        case IDEN:
            tmp = new IdentifierExp(pre.iden);
            expStack.push(tmp);
            break;
        default: break;
        }
    }

    if(opStack.isEmpty() && expStack.length() == 1) {
        return expStack.pop();
    }

    while (!opStack.isEmpty() && !expStack.isEmpty()) {
        token top = opStack.pop();
        if(top.getType() == NEGATIVE) {
            Expression* operand = expStack.pop();
            Expression* constant0 = new ConstantExpression(0);
            tmp = new CompoundExp(top, constant0, operand);
        }
        else {
            Expression* operand2 = expStack.pop();
            Expression* operand1 = expStack.pop();
            tmp = new CompoundExp(top, operand1, operand2);
        }
        if(opStack.isEmpty() && expStack.isEmpty()) {
            root = tmp;
            break;
        }
        expStack.push(tmp);
    }

    return root;
}
