#pragma once

#include <vector>
#include <string>

#ifdef _WIN32
    #include <windows.h>
#endif

// 跨平台获取 UTF-8 编码的命令行参数列表
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
