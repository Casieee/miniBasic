#ifndef PROGRAM_H
#define PROGRAM_H

#include <QWidget>
#include "statement.h"
#include <QMap>
#include "exp.h"

class program
{
public:
    program();
    ~program();

    void executeCommand(QStringList cmdParts);
    void endProgram();
    void clear();

    void ChangeContext(QString name, int value);

    bool ifEnd;
    bool newResult;
    bool ifInput;
    int result;
    int executeLine;
    QString inputIden;

private:
    Statement* stat;
    tokenize tool;
    EvaluationContext context;
};

#endif // PROGRAM_H
