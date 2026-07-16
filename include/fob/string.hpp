#pragma once

#include<string>
#include<string_view>

namespace fob{

/**
 * @brief 将 UTF-8 编码的字节序列转换为 std::u8string.
 * @param utf8_sv 必须是合法的 UTF-8 编码字节序列,否则行为未定义.
 * @return 与输入内容相同的 std::u8string(字节不变,仅 reinterpret_cast).
 */
std::u8string to_u8str(std::string_view utf8_sv);

/**
 * @brief 将 std::u8string 转换为 std::string(按原始字节存储).
 * @param u8str 输入必须是合法的 UTF-8 序列(但本函数不校验).
 * @note 若在非 UTF-8 环境下使用,结果可能不符合预期.
 */
std::string to_str(std::u8string const& u8str);

}//namespace fob
