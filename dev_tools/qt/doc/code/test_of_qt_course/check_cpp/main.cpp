#include <iostream>

using namespace std;

int main()
{
    int i = 10;
    int &ri = i;
    ri = 100;
    int *pi = &i;
    int *pri = &ri;
    *pri = 90;

    cout << "ri = " << ri << endl;
    cout << "i = " << i << endl;

    return 0;
}
