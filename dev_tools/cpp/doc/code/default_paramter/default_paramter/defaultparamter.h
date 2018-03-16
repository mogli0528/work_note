#ifndef DEFAULTPARAMTER_H
#define DEFAULTPARAMTER_H

#include <iostream>

using namespace std;

class DefaultParamter
{
public:
    DefaultParamter() { }

    virtual void Solve(const char* resume_file = NULL);
};

#endif // DEFAULTPARAMTER_H
