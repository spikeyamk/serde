#include "serde/serializer.hpp"
#include "serde/deserializer.hpp"

namespace Serde {
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

        struct Super {
            uint64_t a;
        };

        template<typename ... Args>
        struct pack_holder {
            template <template < typename ... > typename apply_to_T>
            using apply_to = apply_to_T<Args...>;
        };

        using ObjectSuperPack = pack_holder<Object, Super>;
        using ObjectSuperSerializer = ObjectSuperPack::apply_to<Serde::Serializer>;
        using ObjectSuperDeserializer = ObjectSuperPack::apply_to<Serde::Deserializer>;

        template<typename T>
        bool test(const T& obj) {
            const auto ser { ObjectSuperSerializer::run(obj) };
            const auto de { ObjectSuperDeserializer::run<T>(ser) };

            if(boost::pfr::eq(obj, de)) {
                return true;
            } else {
                return false;
            }
        }

        int object_super() {
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

            if(test(object) == false) {
                return -1;
            }

            const Super super {
                .a = 444,
            };

            if(test(super) == false) {
                return -2;
            }

            return 0;

        }
    }
}

int main() {
    return Serde::Tests::object_super();
}