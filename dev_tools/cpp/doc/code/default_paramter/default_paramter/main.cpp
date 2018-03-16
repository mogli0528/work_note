#include <defaultparamter.h>

int main()
{
    DefaultParamter *dp = new DefaultParamter();
    dp->Solve();
    dp->Solve("caffemodel");
    return 0;
}

