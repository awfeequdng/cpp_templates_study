#include <iostream>

using namespace std;

int main() {
    std::decay_t<int> a = 3;

    cout << typeid(a).name() << endl;

    return 0;
}