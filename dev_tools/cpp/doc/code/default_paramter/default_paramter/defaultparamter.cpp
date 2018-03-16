#include "defaultparamter.h"

void DefaultParamter::Solve(const char* resume_file) {
    if (NULL == resume_file)
        cout << "no parameter" << endl;
    else
        cout << "parameter is " << resume_file << endl;

}
