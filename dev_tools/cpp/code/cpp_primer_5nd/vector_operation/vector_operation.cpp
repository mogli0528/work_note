#include <vector>
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
    // 本编译器的 insert() 返回值为 void  
    ivec.insert(ivec.begin() + 5, 4, 34);
    ivec.insert(ivec.begin(), ivec_il.begin(), ivec_il.end());
    ivec.insert(ivec.begin(), {33, 44, 55, 66});
    for(auto it = ivec.begin(); it != ivec.end(); it++){

        if( (it - ivec.begin()) % 6 == 0 )
            cout << endl;
        cout << setiosflags(ios::left) << setw(4) << *it << " "; 
    }

    cout << endl;
    // 


    /// 类型转换   
    double d = 0.341;
    auto s = to_string(d);
    d = stod(s);
    s = "1p = 0.122ss";
    size_t pos;
    auto i = stoi(s.substr(s.find(".")+1), &pos, 10);
    cout << "s = " << s << endl;
    cout << "d = " << d << endl;
    cout << "i = " << i << endl;
    cout << "p = " << pos << endl;   // 返回表示整数的字符子串后第一个非数值字符的下标.   

    return 0;
}


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