#include <utility>
#include <iostream>
class X {
public:
    int i = 0;
};

void g (X&) {
    std::cout << "g() for variable\n";
}

void g (X const&) {
    std::cout << "g() for constant\n";
}

void g (X&&) {
    std::cout << "g() for movable object\n";
}

// let f() perfect forward argument val to g():
template<typename T>
void f (T&& val) {
    g(std::forward<T>(val)); // call the right g() for any passed argument val
}

/*
不要以为模板参数 T 的 T&&和具体类型 X 的 X&&是一样的。
虽然语法上看上去类似，但是 它们适用于不同的规则：
  - 具体类型 X 的 X&&声明了一个右值引用参数。只能被绑定到一个可移动对象上（一个 prvalue，
    比如临时对象，一个 xvalue，比如通过 std::move()传递的参数，更多细节参见 附录 B）。
    它的值总是可变的，而且总是可以被“窃取”。
  - 模板参数 T 的 T&&声明了一个转发引用（亦称万能引用）。可以被绑定到可变、不可 变
    （比如 const）或者可移动对象上。在函数内部这个参数也可以是可变、不可变或者 指向一个可以被
    窃取内部数据的值。 注意 T 必须是模板参数的名字。只是依赖于模板参数是不可以的。对于模板
    参数 T，形如 typename T::iterator&&的声明只是声明了一个右值引用，不是一个转发引用。
*/

int main() {
    X v;
    X const c; // create constant
    f(v); // f() for variable calls f(X&) => calls g(X&)
    f(c); // f() for constant calls f(X const&) => calls g(X const&)
    f(X()); // f() for temporary calls f(X&&) => calls g(X&&)
    f(std::move(v)); // f() for move-enabled variable calls f(X&&)=> calls g(X&&)
}