#pragma once

#include <cstddef>
#include <bit>
#include <type_traits>
#include <array>
#include <algorithm>

namespace fob {

template <typename T>
    requires std::is_trivially_copyable_v<T>
constexpr T host_to_net(T host_data) {
    if constexpr (std::endian::native == std::endian::big) {
        return host_data;
    } else {
        auto bytes =
            std::bit_cast<std::array<std::byte, sizeof(T)>>(host_data);
        std::reverse(bytes.begin(), bytes.end());  // 直接反转整个数组
        return std::bit_cast<T>(bytes);
    }
}

template <typename T>
    requires std::is_trivially_copyable_v<T>
constexpr T net_to_host(T net_data) {
    return host_to_net(net_data);
}

} // namespace fob
