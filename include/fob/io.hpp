#pragma once

#include<vector>
#include<string>
#include<string_view>

namespace fob{

/**
 * @brief 跨平台获取 main() 函数的命令行参数,并转换为 UTF-8 编码.
 * 
 * 在 Linux/macOS 下直接拷贝 argv 的字节(需系统 locale 为 UTF-8);
 * 在 Windows 下通过 GetCommandLineW() 获取宽字符并转码,避免 GBK 乱码.
 * 
 * @param argc main 函数的 argc 参数.
 * @param argv main 函数的 argv 参数.
 * @return std::vector<std::string> 每个元素均为 UTF-8 编码的参数字符串.
 * @throws std::bad_alloc 若内存分配失败.
 * @note 返回的字符串拥有独立副本,不受原 argv 生命周期影响.
 */
std::vector<std::string> get_args(int argc, char* argv[]);

/**
 * @brief 跨平台输出 UTF-8 字符串到标准输出(不带换行).
 * 
 * 支持控制台和重定向文件:
 * - Linux/macOS:直接写入 stdout.
 * - Windows:自动设置控制台为 UTF-8 模式,并优先使用宽字符 API 以保证正确显示.
 * 
 * @param str 要输出的 UTF-8 字符串(std::string_view 可接受多种类型).
 * @pre str 必须为有效的 UTF-8 字节序列,否则输出未定义.
 * @note 若输出到文件,写入原始 UTF-8 字节(无 BOM).
 */
void print(std::string_view str);

/**
 * @brief 跨平台输出 UTF-8 字符串到标准输出,并自动追加换行符.
 * 
 * 行为同 print(),但会在末尾输出 '\n'(跨平台统一为 \n).
 * 
 * @param str 要输出的 UTF-8 字符串.
 * @see print()
 */
void println(std::string_view str);

}//namespace fob
