#include <fob/io.hpp>

// 测试函数：验证命令行参数解析和 UTF8 输出
void test_utf8_utils(int argc, char* argv[]) {
    using namespace fob::io;
    println_utf8("========== 测试 get_utf8_argv ==========");

    auto args = get_utf8_argv(argc, argv);
    println_utf8("参数个数 (argc): " + std::to_string(args.size()));

    for (size_t i = 0; i < args.size(); ++i) {
        // 构建消息：索引 + 参数值
        std::string msg = "argv[" + std::to_string(i) + "] = ";
        // 将 std::u8string 转为 std::string_view（不复制）
        std::string_view sv(reinterpret_cast<const char*>(args[i].data()), args[i].size());
        msg += sv;
        println_utf8(msg);
    }

    println_utf8("\n========== 测试 print_utf8 ==========");
    print_utf8("普通 ASCII: Hello, world!\n");
    print_utf8("中文: 你好，世界！\n");
    print_utf8("日文: こんにちは\n");
    print_utf8("Emoji: 😄 🎉 🚀\n");
    print_utf8("特殊符号: © ® € £ ¥\n");
    print_utf8("混合: 文件路径 C:/用户/测试/文件.txt\n");

    println_utf8("\n如果以上内容显示正常，说明一切工作正常。");
}

// 主函数
int test_fob_io(int argc, char* argv[]) {
    test_utf8_utils(argc, argv);
    return 0;
}
