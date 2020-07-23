#pragma once
#include <e/system/basic_type.h>
#include <e/system/methodptr_caller.h>
#include <thread>
namespace e::lib::spec::MethodPtrUtils
{
    inline intptr_t GetMethodAddress(e::system::methodptr x)
    {
        return (intptr_t)x;
    }
    inline intptr_t GetMethodAddress(intptr_t x)
    {
        return x;
    }
    inline intptr_t GetMethodAddress(const e::system::any &x)
    {
        if (x.type() == typeid(intptr_t))
        {
            return x.cast<intptr_t>();
        }
        else
        {
            return (intptr_t)x.cast<e::system::methodptr>();
        }
    }

    template <typename TPtr, typename TReserve, typename TResult, typename... TArgs>
    bool Call(TPtr &&ptr, TReserve &&reserve, std::optional<std::reference_wrapper<TResult>> result, TArgs &&... args)
    {
        using realResultType = std::conditional_t<
            std::is_same_v<std::remove_const_t<std::remove_reference_t<typename TResult>>, e::system::any>,
            intptr_t,
            typename TResult>;
        e::system::MethodPtrCaller<typename realResultType(typename std::remove_const_t<std::remove_reference_t<typename TArgs>>...)> caller;
        auto realResult = caller.call((void *)GetMethodAddress(std::forward<TPtr>(ptr)), std::forward<TArgs>(args)...);
        if (result.has_value())
        {
            result.value().get() = static_cast<TResult>(realResult);
        }
        return true;
    }
    template <typename TPtr, typename TReserve, typename TResult, typename... TArgs>
    bool Call(TPtr &&ptr, TReserve &&reserve, std::reference_wrapper<TResult> result, TArgs &&... args)
    {
        return Call(std::forward<TPtr>(ptr), std::forward<TReserve>(reserve), std::optional(result), std::forward<TArgs>(args)...);
    }
    template <typename TPtr, typename TReserve, typename... TArgs>
    bool Call(TPtr &&ptr, TReserve &&reserve, std::nullopt_t result, TArgs &&... args)
    {
        return Call(std::forward<TPtr>(ptr), std::forward<TReserve>(reserve), std::optional<std::reference_wrapper<intptr_t>>(result), std::forward<TArgs>(args)...);
    }
    template <typename TPtr, typename TReserve>
    bool Call(TPtr &&ptr, TReserve &&reserve)
    {
        return Call(std::forward<TPtr>(ptr), std::forward<TReserve>(reserve), std::nullopt);
    }
    template <typename TPtr>
    bool Call(TPtr &&ptr)
    {
        return Call(std::forward<TPtr>(ptr), std::nullopt);
    }

    template <typename TCallParams>
    bool DelayCall(int32_t delay, TCallParams &&params)
    {
        std::thread([=] {
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            Call(std::forward<TCallParams>(params)...)
        }).detach();
        return true;
    }
}