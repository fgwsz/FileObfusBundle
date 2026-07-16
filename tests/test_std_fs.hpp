#pragma once

// 源代码文件的编码类型是UTF8
#include <fstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

//创建文件使用path(u8string)
//写入文件需要编码转换
void test_std_fs() {
    // 方案2：如果使用MSVC且添加了 /utf-8 编译选项，也可以用 u8""
    fs::path p(u8"你好世界/这是一个测试路径/😄.txt");

    // 创建父目录
    if (!fs::exists(p.parent_path())) {
        if (!fs::create_directories(p.parent_path())) {
            std::cout << "Create Directories Error\n";
            return;
        }
    }

    std::ofstream ofs(p,std::ios::binary);
    if (!ofs) {
        std::cout << "Create File Error\n";  // 补上分号
        return;
    }
    std::u8string context=u8"你好世界";
    //ofs<<std::string("你好世界");//使用operator<<()非二进制写入,是gbk编码
    ofs.write((char*)&(context[0]),context.size());//使用write二进制写入就是utf8编码了
    ofs.close();

    std::cout << "Create File Passed\n";
    //最终结果:成功创建了文件路径,但是写入的内容字符串是GBK编码
}
//二进制读取和写入不涉及编码问题
//创建文件/目录路径使用fs::path(std::u8string)可以规避编码问题
//现在还有一个问题:控制台读入的参数路径如何转为fs::path(std::u8string)
