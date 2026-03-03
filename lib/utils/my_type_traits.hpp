#pragma once

#ifndef __SIZEOF_INT128__
#error "This library requires compiler support for __int128."
#endif

namespace internal {

template <typename T, typename U>
concept same_as = std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;

template <typename T>
concept basic_signed_integral =
    std::signed_integral<std::remove_cvref_t<T>> && !same_as<T, char>;

template <typename T>
concept basic_unsigned_integral =
    std::unsigned_integral<std::remove_cvref_t<T>> && !same_as<T, bool>;

template <typename T>
concept basic_integral = basic_signed_integral<T> || basic_unsigned_integral<T>;

template <typename T>
concept signed_integral = basic_signed_integral<T> || same_as<T, __int128_t>;

template <typename T>
concept unsigned_integral =
    basic_unsigned_integral<T> || same_as<T, __uint128_t>;

template <typename T>
concept integral = signed_integral<T> || unsigned_integral<T>;

template <typename T>
struct make_unsigned : std::make_unsigned<std::remove_cvref_t<T>> {};

template <typename T>
  requires(same_as<T, __int128_t> || same_as<T, __uint128_t>)
struct make_unsigned<T> {
  using type = __uint128_t;
};

template <typename T>
using make_unsigned_t = typename make_unsigned<T>::type;

}  // namespace internal