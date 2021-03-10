#include "program.h"

program::program():
    ifEnd(false), newResult(false), ifInput(false),
    result(0), executeLine(0), inputIden("")
{

}

program::~program() {}

void program::executeCommand(QStringList cmdParts) {
    QString statLine;
    statLine.append(cmdParts[0]);
    statLine.append(" ");
    statLine.append(cmdParts[1]);
    tree.append(statLine);

    cmdParts.removeFirst();
    QString type = cmdParts.takeFirst().trimmed();

    QList<token> exp;

    if(type == "REM") {
        QString rem;
        rem.append('\t');
        for(auto part: cmdParts) {
            rem.append(part);
            rem.append(" ");
        }
        tree.append(rem);
        return;
    }
    if(type == "LET") {
        exp = tool.analyze(cmdParts);
        stat = new LetStatement(exp, context);
        QStringList tmp = stat->toString();
        for(int i = 0; i < tmp.length(); i++) {
            tmp[i].insert(0,'\t');
        }
        tree.append(tmp);
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
            QString tmp;
            tmp.append('\t');
            tmp.append(QString::number(executeLine));
            tree.append(tmp);
        }
        return;
    }
    if(type == "IF") {
        QStringList ifString, thenString = cmdParts, treeString;
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

        treeString = stat->toString();
        for(int i = 0; i < treeString.length(); i++) {
            treeString[i].insert(0,'\t');
        }
        tree.append(treeString);
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
    tree.clear();
}

QStringList program::getTree() {
    return tree;
}
