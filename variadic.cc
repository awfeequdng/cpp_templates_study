#include <iostream>
using namespace std;

template <typename T>
void print(T t) {
    cout << t << endl;
}

template <class T, class... Args>
void print(T t, Args... args) {
    cout << t << endl;
    print(args...);
}

template <class T, class... Args>
void show(T t, Args... args) {
    if constexpr (sizeof...(args) == 0) {
        cout << t << endl;
    } else {
        cout << t << endl;
        show(args...);
    }
}

template <class... Args>
void display(Args... args) {
    ((cout << args << " "), ...);
    cout << endl;
}

int main() {

    print(3, 3.1, "hello wolrd");
    show(3, 3.11, "hello wolrd");
    display(32, 3.111, "hello2 wolrd");

    return 0;
}