#include "TypeTraits.hh"

class A {
public:
    bool operator()() {
        return 0;
    }
};
class B {
public:
    bool operator==(const B&) {
        return true;
    }
};

namespace std
{
    template<>
    struct hash<B> final {
        size_t operator()(const B&) {
            return 0;
        }
    };
} // namespace std

bool boolfunc(const B&) { return 0;}

template <class T>
struct NoCondition{};

int main() {
    std::cout << "is int equality: " << is_equality_comparable<int>::value << std::endl;
    std::cout << "is A equality: " << is_equality_comparable<A>::value << std::endl;
    std::cout << "is B equality: " << is_equality_comparable<B>::value << std::endl;

    std::cout << "is int hashable: " << is_hashable<int>::value << std::endl;
    std::cout << "is A hashable: " << is_hashable<A>::value << std::endl;
    std::cout << "is B hashable: " << is_hashable<B>::value << std::endl;

    std::cout << "is int function_type: " << is_function_type<int>::value << std::endl;
    std::cout << "is A function_type: " << is_function_type<A>::value << std::endl;
    using functor = void(void);
    std::cout << "is functor function_type: " << is_function_type<functor>::value << std::endl;

    std::cout << "is instantiation of vector " << is_instantiation_of<std::vector, std::vector<int>>::value << std::endl;
    std::cout << "is instantiation of vector " << is_instantiation_of<std::vector, std::is_array<int>>::value << std::endl;

    // 不知道如何全面使用strip_class_t
    // strip_class_t<decltype(&B::operator==)> t;
    // using functype = function_traits<strip_class_t<decltype(&B::operator==)>>;
    // functype::func_type t = &boolfunc;
    // B b;
    // std::cout << "b = " << t(b) << std::endl;
    using functype = function_traits<strip_class_t<decltype(&B::operator==)>>;
    functype::return_type cc = 0;
    std::cout << "cc = " << cc << "; typename of cc is: " <<typeid(cc).name() << std::endl;

    std::cout << "int is functor: " << is_functor<int>::value << std::endl;
    std::cout << "A is functor: " << is_functor<A>::value << std::endl;
    std::cout << "B is functor: " << is_functor<B>::value << std::endl;
    auto functor_var = [] () {return 0;};
    std::cout << "functor_var is functor: " << is_functor<decltype(functor_var)>::value << std::endl;
    std::cout << "std::decay_t<decltype(functor_var) is functor: " << is_functor<std::decay_t<decltype(functor_var)>>::value << std::endl;

    auto lamda_1 = []() {return 0;};
    static_assert(std::is_same<decltype(lamda_1), decltype(lamda_1)>::value, "");


    std::cout << "is type condition: " << is_type_condition<std::is_reference>::value << std::endl;
    std::cout << "is type condition: " << is_type_condition<NoCondition>::value << std::endl;

    auto stateless_lambda = [](int a) { return a; };
    auto stateless_lambda1 = [=](int a) { return a; };
    static_assert(is_stateless_lambda<decltype(stateless_lambda)>::value, "");
    static_assert(!is_stateless_lambda<decltype(stateless_lambda1)>::value, "");
    static_assert(!is_stateless_lambda<A>::value, "");
    // static_assert(is_functor<A>::value, "");

    // std::cout << "is stateless lambda : " << is_stateless_lambda<decltype(functor_var)>::value << std::endl;
    return 0;
}
