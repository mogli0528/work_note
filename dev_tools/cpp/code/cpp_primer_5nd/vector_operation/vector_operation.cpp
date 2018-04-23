#include <vector>
#include <list>
#include <array>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <limits>

using namespace std;

int main(int argc, char * argv[])
{
    // resize(), size(), capacity(), reserve()
    std::vector<int> ivec(10, 111);
    std::vector<int> ivec_il = {1, 2, 3, 4, 5, 6};
    cout << "size = " << ivec.size() << endl;
    cout << "il_size = " << ivec_il.size() << endl;
    ivec.reserve(30);
    cout << endl << "size = " << ivec.size() << endl;
    cout << "capacity = " << ivec.capacity() << endl;
    ivec.resize(10);
    cout << endl << "size = " << ivec.size() << endl;
    cout << "capacity = " << ivec.capacity() << endl;
    ivec.resize(40);
    cout << endl << "size = " << ivec.size() << endl;
    cout << "capacity = " << ivec.capacity() << endl;

    // insert(p, n, t)
    // insert(p, b, e)
    // insert(p, il)
    // 编译器的 insert() 返回值为 void  
    auto i_v = ivec.insert(ivec.begin() + 5, 4);
    ivec.insert(i_v, 34);
    
    ivec.insert(ivec.begin() + 5, 4, 35);
    ivec.insert(ivec.begin(), ivec_il.begin(), ivec_il.end());
    ivec.insert(ivec.begin(), {33, 44, 55, 66});
    for(auto it = ivec.begin(); it != ivec.end(); it++){

        if( (it - ivec.begin()) % 6 == 0 )
            cout << endl;
        cout << setiosflags(ios::left) << setw(4) << *it << " "; 
    }

    cout << endl;

    // vector
    vector<int> a(10, 3);
    vector<int> b(3, 3);
    cout << a.size() << endl;
    a = b;
    b = {0, 1, 2};
    cout << a.size() << endl;
    if(a == b)
        cout << "a == b" << endl;
    else
        cout << "a != b" << endl;

    // assign
    vector<const char *> old_style;
    list<string> names;
    //names = old_style;  // error: 容器类型不匹配
    names.assign(old_style.cbegin(), old_style.cend());

    list<string> slist1(1);
    // 等价于 slist1.clear(); --> slist1.insert(slist1.begin(), 10, "Hiya");
    slist1.assign(10, "Hiya");

    // array
    array<int, 10> a1 = {0, 1,2,3,4,5,6,7,8,9};
    array<int, 8> a2 = {0};
    array<int, 10> a3 = {0};
    cout << "a2.size()=" << a2.size()<< endl;

    a1 = a3;         // 正确: 替换 a1 中的元素
    // a1 = a2;         // error: a1 和 a2 中的元素个数不同
    a2 = {0, 1, 2};  // 覆盖 a2 中的前 3 个元素
    // a2 = {0, 1,2,3,4,5,6,7,8,9};  // error: 初始化列表中元素个数大于 a2 中的元素个数
    cout << "a2.size()=" << a2.size()<< endl;


    return 0;
}

// 按照精度提取浮点数为字符串  
string do_fraction(long double value, int decplaces = 3)
{
    ostringstream out;
    int prec = numeric_limits<long double>::digits10;    // 18
    out.precision(prec);  //覆盖默认精度
    out<<value;
    string str= out.str(); //从流中取出字符串
    size_t n=str.find(".");
    if ((n!=string::npos) //有小数点吗?
        && (str.size()> n+decplaces))
        //后面至少还有decplaces位吗？ 　
    {
        str[n+decplaces]='\0';//覆盖第一个多余的数
    }
    // str.swap(string(str.c_str()));//删除nul之后的多余字符

    return str;
 }