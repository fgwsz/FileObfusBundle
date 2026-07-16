#pragma once

#include <string>
#include <cstdio>
#include <string_view>

#ifdef _WIN32
    #include <windows.h>
#endif

// 跨平台输出 UTF8 字符串到标准输出(控制台或重定向文件)
inline void print_utf8(const std::string& utf8_str) {
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

// 重载:支持 std::u8string 参数
inline void print_utf8(const std::u8string& utf8_str) {
    // 将 u8string 转为普通 string(仅 reinterpret,不改变数据)
    std::string_view sv(
        reinterpret_cast<const char*>(utf8_str.data()),
        utf8_str.size()
    );
    print_utf8(std::string(sv));
}

// 带换行版本
inline void println_utf8(const std::string& utf8_str) {
    print_utf8(utf8_str);
    print_utf8("\n");
}
inline void println_utf8(const std::u8string& utf8_str) {
    print_utf8(utf8_str);
    print_utf8("\n");
}
