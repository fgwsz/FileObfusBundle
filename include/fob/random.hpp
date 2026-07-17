#pragma once

#include <concepts>

namespace fob {

/**
 * @brief 概念:可用于随机数生成的数字类型.
 * 
 * 包括所有整数类型(不含 bool)和浮点数类型.
 */
template <typename T>
concept random_number =
    std::integral<T> && !std::same_as<T, bool> || std::floating_point<T>;

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
Number random(Number begin, Number end, Generator& gen);

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
Number random(Number begin, Number end);

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
Number random_closed(Number begin, Number finish, Generator& gen);

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
Number random_closed(Number begin, Number finish);

} // namespace fob

#include<fob/internal/random_impl.hpp>
