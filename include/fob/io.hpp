#pragma once

#include<vector>
#include<string>

namespace fob::io{

// 跨平台获取 UTF-8 编码的命令行参数列表
std::vector<std::u8string> get_utf8_argv(int argc, char* argv[]);
// 跨平台输出 UTF8 字符串到标准输出(控制台或重定向文件)
void print_utf8(std::string const& utf8_str);
// 重载:支持 std::u8string 参数
void print_utf8(std::u8string const& utf8_str);
// 带换行版本
void println_utf8(std::string const& utf8_str);
void println_utf8(std::u8string const& utf8_str);

}//namespace fob::io
