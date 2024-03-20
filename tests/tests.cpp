#include <array>

#include "serde/serializer.hpp"
#include "serde/deserializer.hpp"

#include "serde/tests.hpp"

namespace Serde {
    namespace Tests {
        template<typename T_Serializer, typename T_Deserializer, typename T>
        bool run(const T& obj) {
            const auto ser { T_Serializer::run(obj) };
            const auto de { T_Deserializer::run<T>(ser) };

            std::cout
                << "typeid(T).name(): "
                << typeid(T).name()
                << std::endl;

            if(boost::pfr::eq(obj, de)) {
                return true;
            } else {
                return false;
            }
        }

        template<typename ... Args>
        struct pack_holder {
            template <template < typename ... > typename apply_to_T>
            using apply_to = apply_to_T<Args...>;
        };
    }

    namespace Tests {
        struct Object {
            uint64_t a;
            uint32_t b;
            uint16_t c;
            uint8_t  d;
            int8_t   e;
            int16_t  f;
            int32_t  g;
            int64_t  h;
            float    i;
            double   j;
        };

        const Object object{
            .a = 1234567890,
            .b = 4294967295,
            .c = 65535,
            .d = 132,
            .e = 64,
            .f = -128,
            .g = -32768,
            .h = -9272036854775808,
            .i = 13.12f,
            .j = 66.643,
        };

        enum class Enum {
            First,
            Second,
        };

        struct Super {
            uint64_t a;
            Enum     b;
        };

        const Super super {
            .a = 444,
            .b = Enum::Second
        };
    }

    namespace Tests {
        int object_super() {
            using ObjectSuperPack = pack_holder<Object, Super>;
            using ObjectSuperSerializer = ObjectSuperPack::apply_to<Serde::Serializer>;
            using ObjectSuperDeserializer = ObjectSuperPack::apply_to<Serde::Deserializer>;

            if(run<ObjectSuperSerializer, ObjectSuperDeserializer>(object) == false) {
                return -1;
            }

            if(run<ObjectSuperSerializer, ObjectSuperDeserializer>(super) == false) {
                return -2;
            }

            return 0;
        }
    }

    namespace Tests {
        struct Iterable {
            template<uint8_t ... Indexes> 
            static auto make_array(std::integer_sequence<uint8_t, Indexes...>) -> std::array<uint8_t, sizeof...(Indexes)> {
                return { Indexes... };
            }

            std::array<uint8_t, 12> inner;
        };

    }

    namespace Tests {
        int object_super_iterable() {
            const Iterable iterable {
                .inner = { Iterable::make_array(std::make_integer_sequence<uint8_t, sizeof(Iterable::inner)>()) }
            };

            std::printf("sizeof(Object): %zu\n", sizeof(Object));
            std::printf("sizeof(Super): %zu\n", sizeof(Super));
            std::printf("sizeof(Iterable): %zu\n", sizeof(Iterable));

            using ObjectSuperIterablePack = pack_holder<Object, Super, Iterable>;
            using ObjectSuperIterableSerializer = ObjectSuperIterablePack::apply_to<Serde::Serializer>;
            using ObjectSuperIterableDeserializer = ObjectSuperIterablePack::apply_to<Serde::Deserializer>;

            if(run<ObjectSuperIterableSerializer, ObjectSuperIterableDeserializer>(object) == false) {
                return -1;
            }

            if(run<ObjectSuperIterableSerializer, ObjectSuperIterableDeserializer>(super) == false) {
                return -2;
            }

            if(run<ObjectSuperIterableSerializer, ObjectSuperIterableDeserializer>(iterable) == false) {
                return -3;
            }

            return 0;
        }
    }
}