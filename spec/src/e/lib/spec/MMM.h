#pragma once
#include <e/system/basic_type.h>
namespace e::lib::spec::MMM
{
    //MMM stands for Manual Memory Management
    intptr_t Alloc(intptr_t size, std::optional<bool> fillWithZero = std::nullopt);
    void Free(intptr_t ptr);
    template <typename T>
    intptr_t AddressOf(const T &x)
    {
        if constexpr (std::is_same_v<T, e::system::any>)
        {
            return reinterpret_cast<intptr_t>(x.address());
        }
        else
        {
            return reinterpret_cast<intptr_t>(&reinterpret_cast<const int &>(x));
        }
    }
    template <typename T>
    intptr_t DataAddressOf(const T &x)
    {
        if constexpr (std::is_same_v<T, e::system::any>)
        {
            if (x.type() == typeid(e::system::string))
            {
                return reinterpret_cast<intptr_t>(x.cast<e::system::string>().c_str());
            }
            if (x.type() == typeid(e::system::bin) || x.is_e_array_type())
            {
                auto pInfo = *(size_t **)x.address();
                if (pInfo == nullptr || pInfo[0] == 0 || pInfo[1] == 0)
                {
                    return 0;
                }
                return reinterpret_cast<intptr_t>(&pInfo[pInfo[0] + 1]);
            }
            return reinterpret_cast<intptr_t>(x.address());
        }
        else if constexpr (std::is_same_v<T, e::system::string>)
        {
            return reinterpret_cast<intptr_t>(x.c_str());
        }
        else if constexpr (std::is_same_v<T, e::system::bin> || e::system::is_e_array_type_v<T>)
        {
            auto pInfo = (size_t *)x.data;
            if (pInfo == nullptr || pInfo[0] == 0 || pInfo[1] == 0)
            {
                return 0;
            }
            return reinterpret_cast<intptr_t>(&pInfo[pInfo[0] + 1]);
        }
        else
        {
            return reinterpret_cast<intptr_t>(&reinterpret_cast<const int &>(x));
        }
    }
}