#ifndef FP_ENUM_PP_DEF_HPP_
#define FP_ENUM_PP_DEF_HPP_

#include "fp_pp_seq_for_each.hpp"
#include "enum_descriptor.hpp"

#include <array>        // for std::array
#include <cstddef>      // for std::size_t

namespace fp {

    template<std::size_t...>
    struct indices { };

    template<std::size_t N, std::size_t... T>
    struct build_indices : build_indices<(N - 1), (N - 1), T...> { };
    template<std::size_t... T>
    struct build_indices<0, T...> : indices<T...> { };

    constexpr bool streq(char const * l, char const * r) {
        return (*l && *r) ? ((*l == *r) ? streq(l + 1, r + 1) : false) : (!(*l || *r));
    }

    template<typename Enum>
    struct enum_helper {
    private:
        // Although these function are being defined, they should never be called
        // They are here only to enable constexpr evaluation of the public functions

        constexpr static char const * get_name(Enum)
        { return 0; }

        constexpr static Enum get_value(char const * n)
        { return static_cast<Enum>(0); }

        template<typename V>
        constexpr static bool is_valid_entry(V v)
        { return false; }

        template<typename V>
        constexpr static Enum parse(V v)
        { return static_cast<Enum>(0); }

    public:

        template<typename H, typename... T>
        constexpr static char const * get_name(Enum v, H const & head, T const &... tail) {
            return (head.value() == v) ? head.name() : (sizeof...(T) > 0) ? get_name(v, tail...) : throw InvalidEnumValueException<Enum>(v);
        }

        template<typename H, typename... T>
        constexpr static Enum get_value(char const * n, H const & head, T const &... tail) {
            return (streq(head.name(), n)) ? head.value() : (sizeof...(T) > 0) ? get_value(n, tail...) : throw InvalidEnumNameException<Enum>(n);
        }

        template<typename V, typename H, typename... T>
        constexpr static bool is_valid_entry(V v, H const & head, T const &... tail) {
            return ((int)head.value() == v) ? true : (sizeof...(T) > 0) ? is_valid_entry(v, tail...) : false;
        }

        template<typename V, typename H, typename... T>
        constexpr static Enum parse(V v, H const & head, T const &... tail) {
            return ((int)head.value() == v) ? head.value() : (sizeof...(T) > 0) ? parse(v, tail...) : throw EnumParseException<Enum>(v);
        }
    };
}


#define STD_ENUM_ENTRY_WITHOUT_VALUE__(ENUM, NAME)      NAME,
#define STD_ENUM_ENTRY_WITHOUT_VALUE_(ENUM, NAME)       STD_ENUM_ENTRY_WITHOUT_VALUE__(ENUM, NAME)
#define STD_ENUM_ENTRY_WITHOUT_VALUE(ENUM, SPLIT...)    STD_ENUM_ENTRY_WITHOUT_VALUE_(ENUM, SPLIT)

#define STD_ENUM_ENTRY_WITH_VALUE__(ENUM, NAME, VALUE)  NAME = VALUE,
#define STD_ENUM_ENTRY_WITH_VALUE_(ENUM, NAME, VALUE)   STD_ENUM_ENTRY_WITH_VALUE__(ENUM, NAME, VALUE)
#define STD_ENUM_ENTRY_WITH_VALUE(ENUM, SPLIT...)       STD_ENUM_ENTRY_WITH_VALUE_(ENUM, SPLIT)

#define FP_PP_ENUM_STD_ENTRY_1(ENUM, SPLIT...)          STD_ENUM_ENTRY_WITHOUT_VALUE(ENUM, SPLIT)
#define FP_PP_ENUM_STD_ENTRY_2(ENUM, SPLIT...)          STD_ENUM_ENTRY_WITH_VALUE(ENUM, SPLIT)
#define FP_PP_ENUM_STD_ENTRY__(N, ENUM, SPLIT...)       FP_PP_ENUM_STD_ENTRY_##N(ENUM, SPLIT)
#define FP_PP_ENUM_STD_ENTRY_(N, ENUM, VALUE)           FP_PP_ENUM_STD_ENTRY__(N, ENUM, FP_PP_EXPAND VALUE)
#define FP_PP_ENUM_STD_ENTRY(ENUM, VALUE)               FP_PP_ENUM_STD_ENTRY_(FP_PP_NUM_ARGS VALUE, ENUM, VALUE)

#define FP_PP_ENUM_EXT_ENTRY_WITHOUT_VALUE__(ENUM, NAME)      ::fp::enum_entry<ENUM>(ENUM::NAME, #NAME),
#define FP_PP_ENUM_EXT_ENTRY_WITHOUT_VALUE_(ENUM, NAME)       FP_PP_ENUM_EXT_ENTRY_WITHOUT_VALUE__(ENUM, NAME)
#define FP_PP_ENUM_EXT_ENTRY_WITHOUT_VALUE(ENUM, SPLIT...)    FP_PP_ENUM_EXT_ENTRY_WITHOUT_VALUE_(ENUM, SPLIT)

#define FP_PP_ENUM_EXT_ENTRY_WITH_VALUE__(ENUM, NAME, VALUE)  ::fp::enum_entry<ENUM>(ENUM::NAME, #NAME),
#define FP_PP_ENUM_EXT_ENTRY_WITH_VALUE_(ENUM, NAME, VALUE)   FP_PP_ENUM_EXT_ENTRY_WITH_VALUE__(ENUM, NAME, VALUE)
#define FP_PP_ENUM_EXT_ENTRY_WITH_VALUE(ENUM, SPLIT...)       FP_PP_ENUM_EXT_ENTRY_WITH_VALUE_(ENUM, SPLIT)

#define FP_PP_ENUM_EXT_ENTRY_1(ENUM, SPLIT...)          FP_PP_ENUM_EXT_ENTRY_WITHOUT_VALUE(ENUM, SPLIT)
#define FP_PP_ENUM_EXT_ENTRY_2(ENUM, SPLIT...)          FP_PP_ENUM_EXT_ENTRY_WITH_VALUE(ENUM, SPLIT)
#define FP_PP_ENUM_EXT_ENTRY__(N, ENUM, SPLIT...)       FP_PP_ENUM_EXT_ENTRY_##N(ENUM, SPLIT)
#define FP_PP_ENUM_EXT_ENTRY_(N, ENUM, VALUE)           FP_PP_ENUM_EXT_ENTRY__(N, ENUM, FP_PP_EXPAND VALUE)
#define FP_PP_ENUM_EXT_ENTRY(ENUM, VALUE)               FP_PP_ENUM_EXT_ENTRY_(FP_PP_NUM_ARGS VALUE, ENUM, VALUE)

#define DEFINE_EXT_ENUM(ENUM, ...)                                                              \
    enum class ENUM {                                                                           \
        FP_PP_SEQ_FOR_EACH(FP_PP_ENUM_STD_ENTRY, ENUM, __VA_ARGS__)                             \
    };                                                                                          \
                                                                                                \
    struct ENUM##_descriptor {                                                                  \
    public:                                                                                     \
        using enum_type = ENUM;                                                                 \
        using entry_type = ::fp::enum_entry<enum_type>;                                         \
        using this_type = ENUM##_descriptor;                                                    \
        using const_iterator = entry_type const *;                                              \
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;                   \
        using size_type = std::size_t;                                                          \
    private:                                                                                    \
        constexpr static std::size_t Size = FP_PP_NUM_ARGS(__VA_ARGS__);                        \
                                                                                                \
        constexpr static std::array<entry_type, Size> const _entries                            \
        {                                                                                       \
            {                                                                                   \
                FP_PP_SEQ_FOR_EACH(FP_PP_ENUM_EXT_ENTRY, ENUM, __VA_ARGS__)                     \
            }                                                                                   \
        };                                                                                      \
                                                                                                \
        template<std::size_t... Is >                                                            \
        constexpr static char const * name_of_impl(enum_type v, ::fp::indices<Is...>) {         \
            using std::get;                                                                     \
            return ::fp::enum_helper<enum_type>::get_name(v, get<Is>(_entries)...);             \
        }                                                                                       \
                                                                                                \
        template<std::size_t... Is >                                                            \
        constexpr static enum_type value_of_impl(char const * n, ::fp::indices<Is...>) {        \
            using std::get;                                                                     \
            return ::fp::enum_helper<enum_type>::get_value(n, get<Is>(_entries)...);            \
        }                                                                                       \
                                                                                                \
        template<typename V, std::size_t... Is >                                                \
        static bool try_parse_impl(V val, enum_type & res, ::fp::indices<Is...>) {              \
            using std::get;                                                                     \
            return (::fp::enum_helper<enum_type>::is_valid_entry(val, get<Is>(_entries)...)) ?  \
                ((res = static_cast<enum_type> (val)), true)                                    \
                : false;                                                                        \
        }                                                                                       \
                                                                                                \
        template<typename V, std::size_t... Is >                                                \
        constexpr static enum_type parse_impl(V val, ::fp::indices<Is...>) {                    \
            using std::get;                                                                     \
            return (::fp::enum_helper<enum_type>::parse(val, get<Is>(_entries)...));            \
        }                                                                                       \
                                                                                                \
        template<typename V, std::size_t... Is >                                                \
        constexpr static bool is_valid_impl(V val, ::fp::indices<Is...>) {                      \
            using std::get;                                                                     \
            return (::fp::enum_helper<enum_type>::is_valid_entry(val, get<Is>(_entries)...));   \
        }                                                                                       \
    public:                                                                                     \
        constexpr ENUM##_descriptor() = default;                                                \
        ENUM##_descriptor(ENUM##_descriptor const &) = delete;                                  \
        ENUM##_descriptor(ENUM##_descriptor &&) = delete;                                       \
                                                                                                \
        constexpr static char const * name() noexcept {                                         \
            return #ENUM;                                                                       \
        }                                                                                       \
                                                                                                \
        constexpr static char const * name_of(enum_type value) {                                \
            return name_of_impl(value, ::fp::build_indices<Size>());                            \
        }                                                                                       \
                                                                                                \
        constexpr static enum_type value_of(char const * name) {                                \
            return value_of_impl(name, ::fp::build_indices<Size>());                            \
        }                                                                                       \
                                                                                                \
        constexpr static size_type size() noexcept {                                            \
            return Size;                                                                        \
        }                                                                                       \
                                                                                                \
        constexpr static const_iterator begin() {                                               \
            using std::get;                                                                     \
            return const_iterator(&get<0>(_entries));                                           \
        }                                                                                       \
                                                                                                \
        constexpr static const_iterator end() {                                                 \
            using std::get;                                                                     \
            return const_iterator(&get<(Size - 1)>(_entries) + 1);                              \
        }                                                                                       \
                                                                                                \
        template<typename T,                                                                    \
                typename = typename std::enable_if<std::is_integral<T>::value>::type>           \
        static bool try_parse(T value, enum_type & res){                                        \
            return try_parse_impl(value, res, ::fp::build_indices<Size>());                     \
        }                                                                                       \
                                                                                                \
        template<typename T,                                                                    \
                typename = typename std::enable_if<std::is_integral<T>::value>::type>           \
        constexpr static enum_type parse(T value){                                              \
            return parse_impl(value, ::fp::build_indices<Size>());                              \
        }                                                                                       \
                                                                                                \
        template<typename T,                                                                    \
                typename = typename std::enable_if<std::is_integral<T>::value>::type>           \
        constexpr static bool is_valid(T value){                                                \
            return is_valid_impl(value, ::fp::build_indices<Size>());                           \
        }                                                                                       \
    };                                                                                          \
    ENUM##_descriptor get_descriptor_mapping(ENUM);                                             \
    constexpr std::array<::fp::enum_entry<ENUM>, FP_PP_NUM_ARGS(__VA_ARGS__)> const ENUM##_descriptor::_entries;

#endif