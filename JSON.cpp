#include <tuple>
#include <map>
#include <iostream>

//sequence for
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

        Value& operator[] (std::string name)
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

    template<typename T> T& asAny(Value&);
    template<typename T> const T& asAny(const Value&);

    template<>
    int& asAny<int>(Value& value) { return value.data.number; }

    template<>
    std::string& asAny<std::string>(Value& value) { return value.data.string; }
}

template<typename Class, typename T>
struct PropertyImpl
{
    constexpr PropertyImpl(T Class::*aMember, const char* aName) : member{aMember}, name{aName}{}

    using Type = T;

    T Class::*member;
    const char* name;
};

template<typename Class, typename T>
constexpr auto property(T Class::*member, const char* name)
{
    return PropertyImpl<Class, T>{member, name};
}

//unserialize function
template<typename T>
T fromJson(const Json::Value& data)
{
    T object;
    //get number of properties
    constexpr auto nbProperties = std::tuple_size<decltype(T::properties)>::value;

    //iterate on the index of nbProperties size
    for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i){
        //get property
        constexpr auto property = std::get<i>(T::properties);
        // get type of property
        using Type = typename decltype(property)::Type;
        //set value to member
        object.*(property.member) = Json::asAny<Type>(data[property.name]);
    });
    return object;
}

//same but for data
template<typename T>
Json::Value toJson(const T& object)
{
    Json::Value data;
    //get number of properties
    constexpr auto nbProperties = std::tuple_size<decltype(T::properties)>::value;
    //iterate on the index of nbProperties size
    for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i){
        //get property
        constexpr auto property = std::get<i>(T::properties);
        //set value to member
        data[property.name] = object.*(property.member);
    });
    return data;
}
