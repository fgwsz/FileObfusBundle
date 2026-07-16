#pragma once

#include <random>
#include <concepts>
#include <limits>

#include <fob/exception.hpp>

namespace fob {

/**
 * @brief 概念:可用于随机数生成的数字类型.
 * 
 * 包括所有整数类型(不含 bool)和浮点数类型.
 */
template <typename T>
concept random_number =
    std::integral<T> && !std::same_as<T, bool> || std::floating_point<T>;

// -------------------- 默认随机引擎(线程安全本地静态) --------------------

namespace detail {
    inline std::mt19937& default_engine(void) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return gen;
    }
}

// -------------------- 半开区间随机数 [begin, end) --------------------

/**
 * @brief 生成半开区间 [begin, end) 内的随机数(使用用户提供的引擎).
 * 
 * @tparam Number 整数或浮点数类型.
 * @tparam Generator 满足 std::uniform_random_bit_generator 的生成器类型.
 * @param begin 区间下限(包含).
 * @param end 区间上限(不包含).
 * @param gen 随机数生成器引用.
 * @return Number 随机值.
 * 
 * @throws fob::Exception 若 begin >= end.
 */
template <fob::random_number Number, typename Generator>
    requires std::uniform_random_bit_generator<Generator>
inline Number random(Number begin, Number end, Generator& gen) {
    if (begin >= end) {
        throw fob::Exception("begin must be < end");
    }

    if constexpr (std::integral<Number>) {
        std::uniform_int_distribution<Number> distrib(begin, end - 1);
        return distrib(gen);
    } else {
        std::uniform_real_distribution<Number> distrib(begin, end);
        return distrib(gen);
    }
}

/**
 * @brief 生成半开区间 [begin, end) 内的随机数(包含 begin,不包含 end).
 * 
 * @tparam Number 整数或浮点数类型.
 * @param begin 区间下限(包含).
 * @param end 区间上限(不包含).
 * @return Number 随机值.
 * 
 * @throws fob::Exception 若 begin >= end.
 * @note 使用默认 mt19937 引擎.
 */
template <fob::random_number Number>
inline Number random(Number begin, Number end) {
    return random(begin, end, detail::default_engine());
}

// -------------------- 闭区间随机数 [begin, finish] --------------------

/**
 * @brief 生成闭区间 [begin, finish] 内的随机数(使用用户提供的引擎).
 * 
 * @tparam Number 整数或浮点数类型.
 * @tparam Generator 满足 std::uniform_random_bit_generator 的生成器类型.
 * @param begin 区间下限(包含).
 * @param finish 区间上限(包含).
 * @param gen 随机数生成器引用.
 * @return Number 随机值.
 * 
 * @throws fob::Exception 若 begin > finish.
 */
template <fob::random_number Number, typename Generator>
    requires std::uniform_random_bit_generator<Generator>
inline Number random_closed(Number begin, Number finish, Generator& gen) {
    if (begin > finish) {
        throw fob::Exception("begin must be <= finish");
    }

    if constexpr (std::integral<Number>) {
        std::uniform_int_distribution<Number> distrib(begin, finish);
        return distrib(gen);
    } else {
        // 浮点数闭区间:扩展上限至下一个可表示浮点数
        auto upper =
            std::nextafter(finish, std::numeric_limits<Number>::max());
        std::uniform_real_distribution<Number> distrib(begin, upper);
        return distrib(gen);
    }
}

/**
 * @brief 生成闭区间 [begin, finish] 内的随机数(整数和浮点数均支持).
 * 
 * @tparam Number 整数或浮点数类型.
 * @param begin 区间下限(包含).
 * @param finish 区间上限(包含).
 * @return Number 随机值.
 * 
 * @throws fob::Exception 若 begin > finish.
 * @note 使用默认 mt19937 引擎.
 */
template <fob::random_number Number>
inline Number random_closed(Number begin, Number finish) {
    return random_closed(begin, finish, detail::default_engine());
}

} // namespace fob
