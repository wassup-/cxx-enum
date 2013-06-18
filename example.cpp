#include "enum_pp_def.hpp"

#include <iostream>

namespace ext {
    DEFINE_EXT_ENUM(my_1st_enum, (fread, 3), (fwrite), (fflush, fread << 2));
    DEFINE_EXT_ENUM(my_2nd_enum, (fopen), (fclose, 1));
}

int main(int argc, char ** argv) {
    using descriptor_t = fp::DescriptorOf<ext::my_1st_enum>;
    descriptor_t descr;
    std::cout << descr.name() << ":" << std::endl;
    // constexpr evaluation
    constexpr std::size_t size_of_enum = descr.size(); // get size of enum (eg number of entries)
    constexpr char const * name_of_enum = descr.name(); // get name of enum
    constexpr char const * name_of_fread = descr.name_of(descriptor_t::enum_type::fread); // find by value
    constexpr descriptor_t::enum_type value_of_fwrite = descr.value_of("fwrite"); // find by name
    constexpr descriptor_t::enum_type first_entry = descr.begin()->value(); // get first entry
    // runtime evaluation
    descriptor_t::enum_type res;
    if (descr.try_parse(1, res)) {
        std::cout << "1 is a valid value for " << descr.name() << " [" << descr.name_of(res) << ']' << std::endl;
    } else {
        std::cout << "1 is not a valid value for " << descr.name() << std::endl;
    }
    for (auto it = descr.begin(); it != descr.end(); ++it) {
        std::cout << "-- " << descr.name_of(it->value()) << " = " << (int) it->value() << std::endl; // find by value
        std::cout << "-- " << (int) descr.value_of(it->name()) << " = " << it->name() << std::endl; // find by name
    }
    std::cout << std::endl;

    return 0;
}