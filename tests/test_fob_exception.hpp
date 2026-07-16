#pragma once

#include<fob.hpp>

void test_fob_exception(void){
    try{
        throw fob::Exception("这是一个fob::Exception");
    }catch(std::exception const& e){
        fob::println(e.what());
    }
}
