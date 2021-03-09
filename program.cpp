#include "program.h"

program::program():
    ifEnd(false), newResult(false), ifInput(false),
    result(0), executeLine(0), inputIden("")
{

}

program::~program() {}

void program::executeCommand(QStringList cmdParts) {
    cmdParts.removeFirst();
    QString type = cmdParts.takeFirst().trimmed();

    context.display();
    QList<token> exp;

    if(type == "REM") {
        return;
    }
    if(type == "LET") {
        exp = tool.analyze(cmdParts);
        stat = new LetStatement(exp, context);
        delete stat;
        return;
    }
    if(type == "PRINT") {
        exp = tool.analyze(cmdParts);
        stat = new PrintStatement(exp, context);
        newResult = true;
        result = stat->getResult();
        delete stat;
        return;
    }
    if(type == "INPUT") {
        exp = tool.analyze(cmdParts);
        if(exp.length() == 1 && exp[0].getType() == IDEN) {
            ifInput = true;
            inputIden = exp[0].toString();
        }
        return;
    }
    if(type == "GOTO") {
        if(cmdParts.length() == 1) {
            executeLine = cmdParts[0].trimmed().toInt();
        }
        return;
    }
    if(type == "IF") {
        QStringList ifString, thenString = cmdParts;
        QList<token> exp2;
        for(int i = 0; i < cmdParts.length(); i++) {
            if(cmdParts[i].trimmed() == "THEN") {
                thenString.removeFirst();
                break;
            }
            else {
                ifString.append(cmdParts[i].trimmed());
                thenString.removeFirst();
            }
        }
        exp = tool.analyze(ifString);
        exp2 = tool.analyze(thenString);
        stat = new IfStatement(exp, exp2, context);

        if(thenString.length() == 1 && stat->ifFlag()) {
            executeLine = thenString[0].toInt();
        }
        delete stat;
        return;
    }
    if(type == "END") {
        endProgram();
        return;
    }
    throw "NO MATCHING STATEMENT";
}

void program::endProgram() {
    ifEnd = true;
}

void program::ChangeContext(QString name, int value) {
    context.setValue(name, value);
}

void program::clear() {
    ifEnd = false;
    newResult = false;
    result = 0;
    executeLine = 0;
    context.clear();
}
