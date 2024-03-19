#pragma once

#include <iostream>

#include "serde/common.hpp"

namespace Serde {
	template<typename ... Args>
	class Deserializer {
	private:
		template<typename T, const size_t obj_size, const size_t obj_i, const size_t ser_size, const size_t ser_i>
		static void p_inner_deserialize(const std::array<uint8_t, ser_size>& ser, T& ret) {
			boost::pfr::get<obj_i>(ret) = *static_cast<const std::remove_reference<decltype(boost::pfr::get<obj_i>(ret))>::type*>(static_cast<const void*>(ser.data() + ser_i));
			if constexpr(obj_i + 1 < obj_size) {
				p_inner_deserialize<T, obj_size, obj_i + 1, ser_size, ser_i + sizeof(boost::pfr::get<obj_i>(ret))>(ser, ret);
			}
		}

		template<typename T>
		static T p_deserialize(const std::array<uint8_t, get_serialized_size<T>()>& ser) {
			T ret {};
			p_inner_deserialize<T, decltype(boost::pfr::detail::tie_as_tuple(ret))::size_v, 0, get_serialized_size<T>(), 1>(ser, ret);
			return ret;
		}
	public:
		constexpr Deserializer() {
			static_assert((sizeof...(Args) < std::numeric_limits<uint8_t>::max()), "Too many arguments");
		}

		template<typename T, const size_t N>
		static T run(const std::array<uint8_t, N>& ser) {
			static_assert((std::is_same_v<T, Args> || ...), "Serde::run: typename T must be member of parameter pack typename ... Args");
			static_assert((N == get_serialized_size<T>()), "Serde::run: const std::array<uint8_t, N>& ser argument of wrong size must be equal to get_serialized_size<T>()");
			if(ser[0] != get_index<T, Args...>()) {
				std::cerr << "Serde::run: const std::array<uint8_t, N>& ser has wrong header must be ser[0] == get_index<T>()" << std::endl;
				std::abort();
			}
			return p_deserialize<T>(ser);
		}
	};
}