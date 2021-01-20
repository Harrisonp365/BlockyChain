#include <tuple>
#include <map>
#include <iostream>

template <typename T, T... S, typename F>

constexpr void for_sequence(std::integer_sequence<T, S...>,F&& f)
{
    using unpack_t = int[];
    (void)unpack_t{(static_cast<void>(f(std::integral_constant<T, S>{})),0)..., 0};
}

namespace Json
{

    struct Value;

    struct ValueData
    {
        std::map<std::string, Value> subObject;
        std::string string;
        int number = 0;
    };

    struct Value
    {
        ValueData data;

        Value& operator[] (std::string name) const
        {
            auto it = data.subObject.find(std::move(name));

            if(it != data.subObject.end())
            {
                return it->second;
            }
            throw;
        }

        Value& operator=(std::string value)
        {
            data.string = value;
            return *this;
        }

        Value& operator=(double value)
        {
            data.number = value;
            return *this;
        }
    };
}
