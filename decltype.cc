#include <iostream>

using namespace std;

class Base{
public:
    int x;
};

int main(){
    const Base obj{};
    //带有括号的表达式
    decltype(obj.x) a = 0;  //obj.x 为类的成员访问表达式，符合推导规则一，a 的类型为 int
    decltype((obj.x)) b = a;  //obj.x 带有括号，符合推导规则三，b 的类型为 int&。
    //加法表达式
    int n = 0, m = 0;
    decltype(n + m) c = 0;  //n+m 得到一个右值，符合推导规则一，所以推导结果为 int
    decltype(n = n + m) d = c;  //n=n+m 得到一个左值，符号推导规则三，所以推导结果为 int&

    int & e = a;

    a = 3;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    a = 30;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;


    cout << typeid(a).name() << endl;
    cout << typeid(b).name() << endl;
    cout << typeid(c).name() << endl;
    cout << typeid(d).name() << endl;
    cout << typeid(e).name() << endl;

    double f = 3.0;
    decltype(false ? f : a) g = 0;

    cout << typeid(f).name() << endl;

    return 0;
}