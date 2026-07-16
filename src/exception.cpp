#include<format>

#include<fob/exception.hpp>

namespace fob{

Exception::Exception(
    std::string_view message,
    std::source_location const location
):what_str_(
    std::format(
        "file: {}({}:{}): {}"
        ,location.file_name()
        ,location.line()
        ,location.column()
        ,message
    )
){}

char const* Exception::what(void) const noexcept{
    return this->what_str_.c_str();
}

}//namespace fob
