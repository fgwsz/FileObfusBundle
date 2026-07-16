#include<fob/string.hpp>

namespace fob{

std::u8string to_u8str(std::string_view utf8_sv){
    return {
        reinterpret_cast<char8_t const*>(utf8_sv.data()),
        utf8_sv.size()
    };
}

std::string to_str(std::u8string const& u8str){
    return {
        reinterpret_cast<char const*>(u8str.data()),
        u8str.size()
    };
}

}//namespace fob
