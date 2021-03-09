#ifndef PARSE_H
#define PARSE_H
#include "exp.h"
#include "tokenize.h"

class parse
{
public:
    parse();
    Expression* analyzeResult(QList<token> exp);
};

#endif // PARSE_H
