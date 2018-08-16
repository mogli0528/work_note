#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct noSemicolon{
    string nameId;
    string passwd;
};

int main()
{
    // int
    const int i = 10;
    const int &cri = i;
    const int &ii = 100;

    int j = 1;
    const int *p_i = &j;
    int *p_j = const_cast<int*>(p_i);
    int &rj = j;

    rj = 100;
    int *pj = &j;
    int *prj = &rj;
    *prj = 90;


    cout << "ri = " << rj << endl;
    cout << "i = " << j << endl;

    // string
    string s1 = "hello";
    string s2("hello");
    string s3(10, 'c');
    string s4 = "hello";
//    getline(cin, s1);
//    cin >> s1 >> s2;
    s1 = s2 + " world!!";
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s3 = " << s3 << endl;
    cout << "s4 = " << s4 << endl;
    cout << "len of s4 = " << s4.size() << endl;
    for(auto &c : s1){
        if (!ispunct(c)){
            c = toupper(c) ;
//            cout << s1 << endl;
        }
    }

    // struct
    noSemicolon data1;
    data1.nameId = "data1";
    cout << "name1 = " << data1.nameId << endl;

    // vector
    vector<int> dict(10, 1);
    for(vector<int>::iterator it = dict.begin(); it != dict.end(); it++){
        *it = 3;
        cout << *it << endl;
    }

    vector<string> svec(10, "hello");

    return 0;
}
