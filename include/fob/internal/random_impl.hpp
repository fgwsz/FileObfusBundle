#pragma once

#include <random>
#include <concepts>
#include <limits>

#include <fob/exception.hpp>

namespace fob {

namespace detail {

// 默认随机引擎(线程安全本地静态)
std::mt19937& default_engine(void) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

}// namespace detail

template <fob::random_number Number, typename Generator>
    requires std::uniform_random_bit_generator<Generator>
Number random(Number begin, Number end, Generator& gen) {
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

template <fob::random_number Number>
Number random(Number begin, Number end) {
    return random(begin, end, detail::default_engine());
}

template <fob::random_number Number, typename Generator>
    requires std::uniform_random_bit_generator<Generator>
Number random_closed(Number begin, Number finish, Generator& gen) {
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

template <fob::random_number Number>
Number random_closed(Number begin, Number finish) {
    return random_closed(begin, finish, detail::default_engine());
}

} // namespace fob
