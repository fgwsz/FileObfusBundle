#pragma once

#include<exception>
#include<string>
#include<string_view>
#include<source_location>

namespace fob{

/**
 * @brief 自定义异常类,集成 C++20 的源码位置信息.
 *
 * 该类继承自 std::exception,在构造时自动捕获调用点的文件名,行号和列号,
 * 并与用户提供的错误消息一起格式化为完整的错误描述字符串.
 * 格式化后的消息可通过 what() 获取,适用于日志记录和用户反馈.
 *
 * @note 要求 C++20 编译器支持 <source_location> 和 <format>.
 *
 * @warning 构造函数的参数 message 应当为有效的 UTF-8 编码字符串,
 *          否则 what() 返回的字符串可能包含非 UTF-8 字节序列,
 *          影响跨平台输出(如 Windows 控制台).
 */
class Exception:public std::exception{
public:

    /**
     * @brief 构造异常对象,自动记录源码位置.
     *
     * 使用 std::format 将源码位置(文件名|行号|列号)和用户消息
     * 组合为格式:`"file: 文件名(行号:列号): 消息"`.
     *
     * @param message 错误描述信息(建议为 UTF-8 编码).
     * @param location 调用点的源码位置,
     *                 默认使用 std::source_location::current()自动捕获.
     *
     * @throws std::bad_alloc 如果内存不足无法构造内部字符串.
     * @throws std::format_error 如果格式字符串无效
     *                           (本实现中固定格式,不会抛出).
     *
     * @note 构造函数未声明 noexcept,因为可能抛出内存分配异常,
     *       这是异常类的常规设计(允许异常在构造时传递资源不足).
     */
    Exception(
        std::string_view message,
        std::source_location const location=std::source_location::current()
    );

    /**
     * @brief 获取错误描述字符串.
     *
     * @return const char* 指向以空字符结尾的 C 风格字符串,
     *                     内容为完整的错误信息.
     *
     * @note 该函数 noexcept,保证不会抛出异常,符合 std::exception 约定.
     * @note 返回的指针在 Exception 对象生命周期内有效.
     */
    virtual char const* what(void) const noexcept override;

private:

     // 存储完整的格式化错误信息(不可变).
    std::string const what_str_;
};

}//namespace fob
