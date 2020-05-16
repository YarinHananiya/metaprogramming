/*******************************************************************************
 * create array of ranges in compile-time
 * Author: Yarin Hananiya
 * ****************************************************************************/
#include <array>       // std::array
#include <tuple>       // std::tuple
#include <type_traits> // std::is_arithmetic_v
#include <utility>     // std::integer_sequence

#include "utils.hpp" // utils::are_same, utils::is_tuple

template<typename T>
constexpr auto make_pairs_tuple(T first, T second) {
    return std::make_tuple(std::make_pair(first, second));
}

template<typename T, typename... Args>
constexpr auto make_pairs_tuple(T first, T second, Args... args) {
    return std::tuple_cat(make_pairs_tuple(first, second), make_pairs_tuple(args...));
}

template<typename T>
constexpr auto calc_ranges(T first, T last) {
    return last - first + 1;
}

template<typename T, typename... Args>
constexpr auto calc_ranges(T first, T last, Args... args) {
    static_assert(std::is_arithmetic_v<T>);

    return calc_ranges(first, last) + calc_ranges(args...);
}

template<typename T, std::size_t N, typename Tuple, std::size_t... I>
constexpr auto tuple_to_array(const Tuple& t, std::index_sequence<I...>) {
    static_assert(utils::is_tuple_v<Tuple>);

    return std::array<T, N>{std::get<I>(t)...};
}

template<auto T, auto Y, auto... Args>
constexpr auto make_pairs_array() {
    constexpr auto pairsTuple = make_pairs_tuple(T, Y, Args...);
    constexpr auto tupleSize = std::tuple_size_v<decltype(pairsTuple)>;
    static_assert(tupleSize > 0);

    return tuple_to_array<std::tuple_element_t<0, decltype(pairsTuple)>, tupleSize>(
        pairsTuple, std::make_index_sequence<tupleSize>());
}

template<auto First, auto Second>
constexpr auto check_range() {
    return (Second >= First);
}

template<auto First, auto Second, auto... Args, typename = std::enable_if_t<(sizeof...(Args) > 0)>>
constexpr auto check_range() {
    return check_range<First, Second>() && check_range<Args...>();
}

template<auto T, auto Y, auto... Args>
constexpr auto make_ranges_array() {
    static_assert(sizeof...(Args) % 2 == 0);
    static_assert(utils::are_same(T, Y, Args...));
    static_assert(std::is_arithmetic_v<decltype(T)>);
    static_assert(check_range<T, Y, Args...>());

    constexpr auto pairsArr = make_pairs_array<T, Y, Args...>();
    std::array<decltype(pairsArr[0].first), calc_ranges(T, Y, Args...)> rangesArr{};
    std::size_t i = 0;
    for (auto [first, last] : pairsArr) {
        while (first <= last) {
            rangesArr[i++] = first++;
        }
    }

    return rangesArr;
}

int main() {
    constexpr auto array = make_ranges_array<'A', 'Z', 'a', 'z', '0', '9'>();
    static_assert(array[2] == 'C');
    static_assert(array[61] == '9');
    static_assert(array.size() == 62);

    return 0;
}