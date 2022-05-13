#pragma once
#include <type_traits>
#include <iostream>
#include <vector>
#include <array>

template <class T, class Enable=void>
struct is_equality_comparable : std::false_type{};
template <class T>
struct is_equality_comparable<T,
    std::void_t<decltype(std::declval<T&>() == std::declval<T&>())>>
    : std::true_type {};
template <class T>
using is_equality_comparable_t = typename is_equality_comparable<T>::type;

template <typename T, typename Enable=void>
struct is_hashable : std::false_type {};

template <typename T>
struct is_hashable<T, std::void_t<decltype(std::hash<T>()(std::declval<T&>()))>>
    : std::true_type {};

template <typename T>
using is_hashable_t = typename is_hashable<T>::type;


/**
 * is_function_type<T> is true_type iff T is a plain function type (i.e.
 * "Result(Args...)")
 */
template<class T>
struct is_function_type : std::false_type {};
template <class Result, class...Args>
struct is_function_type<Result(Args...)> : std::true_type {};
template <class T>
using is_function_type_t = typename is_function_type<T>::type;


template <template<class...> class Template, class T>
struct is_instantiation_of : std::false_type {};
template <template<class...> class Template, class ...Args>
struct is_instantiation_of<Template, Template<Args...>> : std::true_type {};
template <template<class...> class Template, class T>
using is_instantiation_of_t = typename is_instantiation_of<Template, T>::type;

template <class T>
struct strip_class{};
template <class Class, class Result, class...Args>
struct strip_class<Result (Class::*)(Args...)> {
    using type = Result(Args...);
};
template <class Class, class Result, class...Args>
struct strip_class<Result (Class::*)(Args...) const> {
    using type = Result(Args...);
};
template <class T>
using strip_class_t = typename strip_class<T>::type;

template <class Functor, class = void>
struct is_functor : std::false_type {};
template <class Functor>
struct is_functor<Functor,
    std::enable_if_t<is_function_type<
        strip_class_t<decltype(&Functor::operator())>>::value>>
    : std::true_type {};

/**
 * Access information about result type or arguments from a function type.
 * Example:
 * using A = function_traits<int (float, double)>::return_type // A == int
 * using A = function_traits<int (float, double)>::parameter_types::tuple_type
 * // A == tuple<float, double>
 */
template <class Func>
struct function_traits {
  static_assert(
      !std::is_same<Func, Func>::value,
      "In function_traits<Func>, Func must be a plain function type.");
};
template <class Result, class... Args>
struct function_traits<Result(Args...)> {
  using func_type = Result(Args...);
  using return_type = Result;
//   using parameter_types = typelist::typelist<Args...>;
  static constexpr auto number_of_parameters = sizeof...(Args);
};


template< template<class T> class C, class = void>
struct is_type_condition : std::false_type{};

template<template<class T> class C>
struct is_type_condition<C,
        std::enable_if_t<std::is_same<
            bool, std::remove_cv_t<decltype(C<int>::value)>>::value>>
    : std::true_type {};

template <class LambdaType, class Func>
struct is_stateless_lambda__ {
    static_assert(
        !std::is_same<LambdaType, LambdaType>::value,
        "never step in here"
    );
};

template <class LambdaType, class Result, class C, class... Args>
struct is_stateless_lambda__<LambdaType, Result (C::*)(Args...) const> : std::is_convertible<
    LambdaType, Result(*)(Args...)> {};

template <class LambdaType, class Result, class C, class... Args>
struct is_stateless_lambda__<LambdaType, Result (C::*)(Args...)> : std::is_convertible<
    LambdaType, Result(*)(Args...)> {};

template <class LambdaType, class = void>
struct is_stateless_lambda_ : std::false_type {};
template <class LambdaType>
struct is_stateless_lambda_<LambdaType,
    std::enable_if_t<is_functor<LambdaType>::value>>
    : is_stateless_lambda__<LambdaType, decltype(&LambdaType::operator())> {};
template<class T>
using is_stateless_lambda = is_stateless_lambda_<std::decay_t<T>>;

