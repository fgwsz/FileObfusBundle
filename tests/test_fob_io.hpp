#include <cstddef>
#include <format>

#include <fob.hpp>

// 测试函数：验证命令行参数解析和 UTF8 输出
int test_fob_io(int argc, char* argv[]) {
    fob::println("========== 测试 fob::get_args ==========");

    auto args = fob::get_args(argc, argv);
    fob::println(std::format("参数个数 (argc): {}", args.size()));

    for (std::size_t i = 0; i < args.size(); ++i) {
        fob::println(std::format("argv[{}] = {}", i, args[i]));
    }

    fob::println("\n========== 测试 fob::print ==========");
    fob::print("普通 ASCII: Hello, world!\n");
    fob::print("中文: 你好，世界！\n");
    fob::print("日文: こんにちは\n");
    fob::print("Emoji: 😄 🎉 🚀\n");
    fob::print("特殊符号: © ® € £ ¥\n");
    fob::print("混合: 文件路径 C:/用户/测试/文件.txt\n");

    fob::println("\n如果以上内容显示正常，说明一切工作正常。");
    return 0;
}
