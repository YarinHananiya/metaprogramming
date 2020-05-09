/*******************************************************************************
 * utils
 * Author: Yarin Hananiya
 * ****************************************************************************/

namespace utils {
/*----------------------------------------------------------------------------*/
// is_detected
template<typename...>
using try_to_instantiate = void;
using disregard_this = void;

template<template<typename...> class Expression, typename Attempt, typename... Ts>
struct is_detected : std::false_type {};

template<template<typename...> class Expression, typename... Ts>
struct is_detected<Expression, try_to_instantiate<Expression<Ts...>>, Ts...>
  : std::true_type {};

template<template<typename...> class Expression, typename... Ts>
constexpr bool is_detected_v = is_detected<Expression, disregard_this, Ts...>::value;

/*----------------------------------------------------------------------------*/
// is_tuple
template<typename>
struct is_tuple : std::false_type {};

template<typename... T>
struct is_tuple<std::tuple<T...>> : std::true_type {};

template<typename... T>
constexpr bool is_tuple_v = is_tuple<T...>::value;

/*----------------------------------------------------------------------------*/
// are_same
template<typename T>
constexpr auto are_same(T) {
    return true;
}

template<typename T, typename U>
constexpr auto are_same(T, U) {
    return std::is_same_v<T, U>;
}

template<typename T, typename U, typename... Args>
constexpr auto are_same(T t, U u, Args... args) {
    return are_same(t, u) && are_same(u, args...);
}
} // namespace utils