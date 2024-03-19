#pragma once

#include <boost/pfr.hpp>

namespace Serde {
    template<typename T, const size_t index, typename HeadArg, typename ... TailArgs>
    constexpr size_t inner_get_index() {
        if constexpr(std::is_same_v<T, HeadArg>) {
            return index;
        } else {
            return inner_get_index<T, index + 1, TailArgs...>();
        }
    }

    template<typename T, typename ... Args>
    constexpr size_t get_index() {
        return inner_get_index<T, 0, Args...>();
    }

    template<typename T, const size_t obj_size, const size_t obj_i>
    constexpr void inner_get_serialized_size(const T& obj, size_t& ret) {
        ret += sizeof(std::remove_reference<decltype(boost::pfr::get<obj_i>(obj))>::type);
        if constexpr(obj_i + 1 < obj_size) {
            inner_get_serialized_size<T, obj_size, obj_i + 1>(obj, ret);
        }
    }

    template<typename T>
    constexpr size_t get_serialized_size() {
        T obj{};
        static_assert(decltype(boost::pfr::detail::tie_as_tuple(obj))::size_v != 0, "const T& obj must not be empty");
        size_t ret { 0 };
        inner_get_serialized_size<T, decltype(boost::pfr::detail::tie_as_tuple(obj))::size_v, 0>(obj, ret);
        return ret + 1;
    }
}
