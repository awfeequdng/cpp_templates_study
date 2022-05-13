#include <iostream>
#include "TypeTraits.hh"
template <class... T>
struct false_t : std::false_type {};

template <class ...>
class typelist {
private:
    typelist() = delete;
};

template <class Typelist>
struct size {
    // 只能使用这种方式，由于需要进行模板替换，因此在编译的第一阶段该语句不会报错
    static_assert(false_t<Typelist>::value, "never step here");
    // todo: 注意一下这条
    // 由于在编译的第一阶段就已经确定了值为false，因此编译报错，即使模板没有替换也会报错
    // static_assert(std::false_type::value, "never step here");
};

template <class...Args>
struct size<typelist<Args...>> {
    static constexpr size_t value = sizeof...(Args);
};

template <class Typelist>
struct to_tuple {
    static_assert(false_t<Typelist>::value, "never step here");
};
template <class...Args>
struct to_tuple<typelist<Args...>> {
    using type = std::tuple<Args...>;
};
template <class Typelist>
using to_tuple_t = typename to_tuple<Typelist>::type;

template <class Tuple>
struct from_tuple {
    static_assert(false_t<Tuple>::value, "");
};
template <class...Args>
struct from_tuple<std::tuple<Args...>> {
    using type = typelist<Args...>;
};
template <class Tuple>
using from_tuple_t = typename from_tuple<Tuple>::type;

class MyClass {};
static_assert(
    std::is_same<
        typelist<int, float&, const MyClass&&>,
        from_tuple_t<std::tuple<int, float&, const MyClass&&>>>::value,
    "");
static_assert(std::is_same<typelist<>, from_tuple_t<std::tuple<>>>::value, "");
static_assert(
    std::is_same<
        std::tuple<int, float&, const MyClass&&>,
        to_tuple_t<typelist<int, float&, const MyClass&&>>>::value,
    "");
static_assert(std::is_same<std::tuple<>, to_tuple_t<typelist<>>>::value, "");

template <class T>
struct type_ {
    using type = T;
};
template <class Typelist>
struct map_types_to_values_ {
    static_assert(false_t<Typelist>::value, "");
};
template <class...Args>
struct map_types_to_values_<typelist<Args...>> {
    template <class Func>
    constexpr static std::tuple<std::result_of_t<Func(type_<Args>)>...> call(Func&& func) {
        return std::tuple<std::result_of_t<Func(type_<Args>)>...> {
            std::forward<Func>(func)(type_<Args>())...
        };
    }
};
template<class Typelist, class Func>
constexpr decltype(auto) map_types_to_values(Func&& func) {
    return map_types_to_values_<Typelist>::call(std::forward<Func>(func));
}

struct map_to_size {
    template <class T>
    constexpr size_t operator()(T) {
        return sizeof(typename T::type);
    }
};

template<typename Tuple, std::size_t... Index>
void print_tuple_impl(std::ostream& os, const Tuple& t, std::index_sequence<Index...>)
{
    using swallow = int[]; // guaranties left to right order
    (void)swallow { 0, (void(os << (Index == 0 ? "" : ", ") << std::get<Index>(t)), 0)... };
}
template<typename... Args>
decltype(auto) operator<<(std::ostream& os, const std::tuple<Args...>& t)
{
    os << "(";
    print_tuple_impl(os, t, std::index_sequence_for<Args...>{});
    return os << ")";
}

int main() {
    constexpr auto result = map_types_to_values<typelist<int, double, short, char>>(map_to_size());
    constexpr std::tuple<size_t, size_t, size_t, size_t> expected{4, 8, 2, 1};
    static_assert(std::is_same<decltype(result), decltype(expected)>::value, "");
    static_assert(result == expected, "");
    std::cout << "result: " << result << std::endl;
    std::cout << "expected: " << expected << std::endl;
    // for (int i = 0; i < 4; i++) {
    //     if constexpr (std::get<i>(expected) != std::get<i>(result)) {
    //         std::cout << "std::get<i>(expected) != std::get<i>(result)" << std::endl;
    //     }
    // }
    std::cout << "size of typelist : " << size<typelist<int, double, int>>::value << std::endl;
    return 0;
}