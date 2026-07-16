#pragma once

#include <cstddef>
#include <bit>
#include <type_traits>
#include <array>
#include <algorithm>

namespace fob {

/**
 * @brief 将主机字节序转换为网络字节序(大端).
 *
 * @tparam T 必须是可平凡复制(TriviallyCopyable)的类型.
 * @param host_data 主机序数据.
 * @return T 网络序(大端)数据.
 *
 * @note 如果主机本身就是大端,直接返回原值.
 * @note 使用 std::bit_cast 安全地处理字节序列,避免未定义行为.
 */
template <typename T>
    requires std::is_trivially_copyable_v<T>
inline constexpr T host_to_net(T host_data) {
    if constexpr (std::endian::native == std::endian::big) {
        return host_data;
    } else {
        auto bytes =
            std::bit_cast<std::array<std::byte, sizeof(T)>>(host_data);
        std::reverse(bytes.begin(), bytes.end());  // 直接反转整个数组
        return std::bit_cast<T>(bytes);
    }
}

/**
 * @brief 将网络字节序(大端)转换为主机字节序.
 *
 * @tparam T 必须是可平凡复制(TriviallyCopyable)的类型.
 * @param net_data 网络序数据.
 * @return T 主机序数据.
 *
 * @note 由于字节序转换是对称操作,直接调用 host_to_net.
 */
template <typename T>
    requires std::is_trivially_copyable_v<T>
inline constexpr T net_to_host(T net_data) {
    return host_to_net(net_data);
}

} // namespace fob
