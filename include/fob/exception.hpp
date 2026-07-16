#pragma once

#include<exception>
#include<string>
#include<string_view>
#include<source_location>

namespace fob{

class Exception:public std::exception{
public:
    Exception(
        std::string_view message,
        std::source_location const location=std::source_location::current()
    );
    virtual char const* what(void) const noexcept override;
private:
    std::string const what_str_;
};

}//namespace fob
