#include <cstdio>
#include <string_view>

#ifdef _WIN32
    #include <windows.h>
#endif

#include<fob/io.hpp>

namespace fob::io{

std::vector<std::u8string> get_utf8_argv(int argc, char* argv[]) {
    std::vector<std::u8string> result;

    // 辅助 lambda:将 std::string (UTF-8) 转为 std::u8string
    auto to_u8string = [](const std::string& s) -> std::u8string {
        return std::u8string(
            reinterpret_cast<const char8_t*>(s.data()),
            s.size()
        );
    };

#ifdef _WIN32
    // -------------------- Windows 专用 --------------------
    // 辅助 lambda:将 UTF-16 宽字符串转为 UTF-8 std::string
    auto wstring_to_utf8 = [](const std::wstring& wstr) -> std::string {
        if (wstr.empty()) return {};
        int len = WideCharToMultiByte(
            CP_UTF8, 0,
            wstr.data(), static_cast<int>(wstr.size()),
            nullptr, 0, nullptr, nullptr
        );
        std::string result(len, '\0');
        WideCharToMultiByte(
            CP_UTF8, 0,
            wstr.data(), static_cast<int>(wstr.size()),
            result.data(), len, nullptr, nullptr
        );
        return result;
    };

    // 手动解析命令行(与 CommandLineToArgvW 行为一致,无需 shell32)
    auto parse_command_line =
    [](const wchar_t* cmdline)-> std::vector<std::wstring> {
        std::vector<std::wstring> args;
        std::wstring cur;
        bool in_quotes = false;

        for (const wchar_t* p = cmdline; *p; ++p) {
            if (*p == L'"') {
                // 统计前面反斜杠数量
                int backslash_count = 0;
                const wchar_t* q = p - 1;
                while (q >= cmdline && *q == L'\\') {
                    ++backslash_count;
                    --q;
                }
                // 奇数个反斜杠 → 引号被转义(作为普通字符),不改变引号状态
                if (backslash_count % 2 == 1) {
                    // 保留 (backslash_count/2) 个反斜杠(取整)
                    for (int i = 0; i < backslash_count / 2; ++i)
                        cur.push_back(L'\\');
                    cur.push_back(L'"');  // 引号作为普通字符
                    continue;
                }
                // 偶数个反斜杠 → 引号是元字符
                // 保留一半反斜杠
                for (int i = 0; i < backslash_count / 2; ++i)
                    cur.push_back(L'\\');
                // 切换引号状态
                in_quotes = !in_quotes;
            } else if (!in_quotes && (*p == L' ' || *p == L'\t')) {
                // 空白分隔符:结束当前参数
                if (!cur.empty()) {
                    args.push_back(cur);
                    cur.clear();
                }
                // 跳过连续空白
                while (*(p + 1) == L' ' || *(p + 1) == L'\t')
                    ++p;
            } else {
                // 普通字符
                cur.push_back(*p);
            }
        }
        // 最后一个参数
        if (!cur.empty())
            args.push_back(cur);
        return args;
    };

    // 获取原始命令行并解析
    const wchar_t* cmdline = GetCommandLineW();
    auto wargs = parse_command_line(cmdline);
    for (const auto& warg : wargs) {
        std::string utf8_arg = wstring_to_utf8(warg);
        result.push_back(to_u8string(utf8_arg));
    }

#else
    // -------------------- Linux / macOS --------------------
    // 假定 argv 为 UTF-8 编码
    for (int i = 0; i < argc; ++i) {
        result.push_back(to_u8string(std::string(argv[i])));
    }
#endif

    return result;
}

void print_utf8(std::string const& utf8_str) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole != INVALID_HANDLE_VALUE && hConsole != NULL) {
        DWORD fileType = GetFileType(hConsole);
        // 判断是否为控制台(字符设备)
        if (fileType == FILE_TYPE_CHAR) {
            // 将 UTF8 转为 UTF16
            int wlen = MultiByteToWideChar(
                CP_UTF8, 0,
                utf8_str.data(), static_cast<int>(utf8_str.size()),
                nullptr, 0
            );
            if (wlen > 0) {
                std::wstring wstr(static_cast<size_t>(wlen), L'\0');
                MultiByteToWideChar(
                    CP_UTF8, 0,
                    utf8_str.data(), static_cast<int>(utf8_str.size()),
                    wstr.data(), wlen
                );
                DWORD written;
                WriteConsoleW(
                    hConsole, wstr.data(), static_cast<DWORD>(wstr.size()),
                    &written, nullptr
                );
                return;
            }
        }
    }
    // 非控制台(如重定向到文件)或转换失败,直接写入 UTF8 字节流
    fwrite(utf8_str.data(), 1, utf8_str.size(), stdout);
#else
    // Linux / macOS 终端默认 UTF‑8,直接输出
    fwrite(utf8_str.data(), 1, utf8_str.size(), stdout);
#endif
}

void print_utf8(std::u8string const& utf8_str) {
    // 将 u8string 转为普通 string(仅 reinterpret,不改变数据)
    std::string_view sv(
        reinterpret_cast<const char*>(utf8_str.data()),
        utf8_str.size()
    );
    print_utf8(std::string(sv));
}

void println_utf8(std::string const& utf8_str) {
    print_utf8(utf8_str);
    print_utf8("\n");
}

void println_utf8(std::u8string const& utf8_str) {
    print_utf8(utf8_str);
    print_utf8("\n");
}

}//namespace obfus::io
