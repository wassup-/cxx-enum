#ifndef ENUM_DESCRIPTOR_HPP_
#define ENUM_DESCRIPTOR_HPP_

#include <cstddef>      // for std::size_t
#include <exception>    // for std::exception
#include <iterator>     // for std::reverse_iterator
#include <stdexcept>    // for std::invalid_argument
#include <type_traits>  // for std::underlying_type

namespace fp {

    template<typename Enum>
    struct InvalidEnumNameException : std::invalid_argument {
    private:
        using base_type = std::invalid_argument;
        char const * const _name;
    public:
        constexpr InvalidEnumNameException(char const * name) noexcept
        : base_type("invalid entry name for Enum"), _name(name)
        { }

        constexpr char const * name() const noexcept
        { return _name; }
    };

    template<typename Enum>
    struct InvalidEnumValueException : std::invalid_argument {
    private:
        using base_type = std::invalid_argument;
        Enum const _value;
    public:
        constexpr InvalidEnumValueException(Enum value) noexcept
        : base_type("invalid entry value for Enum"), _value(value)
        { }

        constexpr Enum value() const noexcept
        { return _value; }
    };

    template<typename Enum>
    struct EnumParseException : std::invalid_argument {
    private:
        using base_type = std::invalid_argument;
        int const _value;
    public:
        constexpr EnumParseException(int value)
        : base_type("invalid value for Enum"), _value(value)
        { }

        constexpr int value() const noexcept {
            return _value;
        }
    };

    template<typename Enum>
    struct enum_entry {
    public:
        using enum_type = Enum;
        using underlying_type = int;
    private:
        Enum _value;
        char const * const _name;
    public:

        constexpr enum_entry(Enum value, char const * name) noexcept
        : _value(value), _name(name)
        { }

        constexpr enum_entry(enum_entry const &) noexcept = default;

        constexpr Enum value() const
        { return _value; }

        constexpr char const * name() const
        { return _name; }

        constexpr explicit operator underlying_type() const
        { return static_cast<underlying_type>(_value); }
    };

    namespace detail {

        template<typename T>
        void get_descriptor_mapping(T&&);

        template<typename, typename = void>
        struct enum_descriptor_impl;

        template<typename T>
        struct enum_descriptor_impl<T, typename std::enable_if<!std::is_same<void, decltype(get_descriptor_mapping(std::declval<T&>()))>::value>::type> {
            using type = decltype(get_descriptor_mapping(std::declval<T&>()));
        };
    }

    /**
     * Class template for enum descriptors
     * @param  Enum    type of enum
    */
    template<typename Enum>
    struct enum_descriptor {
    public:
        using enum_type = Enum;
        using descriptor_type = typename detail::enum_descriptor_impl<Enum>::type;
        using entry_type = enum_entry<enum_type>;
        using this_type = enum_descriptor<enum_type>;

        using const_iterator = entry_type const *;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using size_type = std::size_t;
    public:
        constexpr enum_descriptor() = default;

        /**
         * Gets the name of Enum
         * @return name of Enum
        */
        constexpr static char const * name() noexcept;

        /**
         * Gets the name of the enum entry
         * @return  name associated with value
         */
        constexpr static char const * name_of(enum_type);

        /**
         * Gets the value associated with name
         * @return  value associated with name
         */
        constexpr static enum_type value_of(char const *);

        /**
         * Gets the number of entries in Enum
         * @return  number of entries in Enum
         */
        constexpr static size_type size() noexcept;

        /**
         * Returns a pointer to the first entry of Enum
         * @return  pointer to the first entry of Enum
         */
        constexpr static const_iterator begin();

        /**
         * Returns a pointer to the one-past-end entry of Enum
         * @return  pointer to the one-past-end entry of Enum
         */
        constexpr static const_iterator end();

        /**
         * Parses an integral value as an entry of Enum
         * @param   value   value to be parsed as Enum
         */
        template<typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
        constexpr static enum_type parse(T);

        /**
         * Tries to parse an integral value as an entry of Enum
         * @param   value   value to be parsed as Enum
         * @param   res     destination of parsed result
         * @return  whether or not parsing was successful
         */
        template<typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
        constexpr static bool try_parse(T, enum_type &);

        /**
         * Checks whether or not an integral value is a valid value for Enum
         * @param value value to be checked
         * @return whether or not value is a valid value for Enum
        */
        template<typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
        constexpr static bool is_valid(T);
    };

    template<typename Enum>
    using DescriptorOf = typename detail::enum_descriptor_impl<Enum>::type;
}

#endif