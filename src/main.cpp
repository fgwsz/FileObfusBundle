#include"main.hpp"

#include<iostream>
#include<format>

#include"test_std_fs.hpp"
#include"test_std_io.hpp"

void say_hello(void)noexcept{
    std::cout<<"hello world!\n";
}

int main(int argc,char* argv[])noexcept{
    test_std_io(argc,argv);
    //test_std_fs();
    return 0;
}
