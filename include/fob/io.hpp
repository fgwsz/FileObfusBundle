#pragma once

#include<vector>
#include<string>
#include<string_view>

namespace fob{

/**
 * @brief 跨平台获取 main() 函数的命令行参数,并转换为 UTF‑8 编码.
 *
 * 此函数屏蔽了 Windows 与 Unix‑like 系统之间的命令行参数编码差异:
 * - 在 Windows 下,通过 GetCommandLineW() 获取原始宽字符命令行,并手动解析
 *   (遵循 CommandLineToArgvW 的解析规则),最后转换为 UTF‑8 字节流.
 * - 在 Linux / macOS 下,直接拷贝 argv[i] 的字节,假定系统 locale 为 UTF‑8.
 *
 * @param argc main 函数的 argc 参数(参数个数).
 * @param argv main 函数的 argv 参数(参数字符串数组).
 * @return std::vector<std::string> 每个元素均为 UTF‑8 编码的参数字符串.
 *
 * @note 返回的字符串拥有独立副本,不受原始 argv 生命周期影响.
 * @note 必须在 main 函数中调用,且传入的 argc/argv 必须是 C 运行时传入的原始值.
 * @throws std::bad_alloc 若内存分配失败.
 *
 * @warning 在 Windows 下,如果命令行包含非 BMP 字符(如 Emoji),可能因宽字符
 *          转换而丢失代理对,但大多数场景下可正常工作.
 */
std::vector<std::string> get_args(int argc, char* argv[]);

/**
 * @brief 跨平台输出 UTF‑8 字符串到标准输出(stdout),不带换行.
 *
 * 此函数自动适配目标环境:
 * - 在 Windows 控制台(Console)下,将 UTF‑8 转换为 UTF‑16,并调用 WriteConsoleW
 *   输出,确保中文|Emoji 等正确显示(需控制台支持 TrueType 字体).
 * - 在 Windows 重定向到文件或管道时,直接写入 UTF‑8 字节流(无 BOM).
 * - 在 Linux / macOS 下,直接写入 stdout,假定终端 locale 为 UTF‑8.
 *
 * @param str 要输出的 UTF‑8 字符串(std::string_view 可接受多种类型).
 *
 * @pre str 必须为有效的 UTF‑8 字节序列,否则输出结果未定义(可能显示乱码).
 * @note 该函数不添加任何额外换行或空白.
 * @note 在多线程环境下,标准输出本身需由外部同步.
 */
void print(std::string_view str);

/**
 * @brief 跨平台输出 UTF‑8 字符串到标准输出,并自动追加换行符.
 *
 * 行为同 print(),但在末尾输出 '\n'(跨平台统一为 \n,由 C 流处理转换).
 *
 * @param str 要输出的 UTF‑8 字符串.
 *
 * @see print()
 */
void println(std::string_view str);

}//namespace fob
