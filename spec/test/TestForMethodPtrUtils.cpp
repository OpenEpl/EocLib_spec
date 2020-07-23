#include <catch2/catch.hpp>
#include <e/lib/spec/MethodPtrUtils.h>
using namespace e::lib::spec;
int32_t MethodWithAIntArgument(int32_t x)
{
    return x << 1;
}
int32_t MethodWithoutArguments()
{
    return 123;
}
void MethodWithoutResult(int32_t x, int32_t *y)
{
    *y = x << 1;
}
int32_t MethodWithAStringArgument(const char* x)
{
    return strlen(x);
}
TEST_CASE("Call a methodptr", "[MethodPtrUtils]")
{
    int32_t x;
    MethodPtrUtils::Call((e::system::methodptr)&MethodWithAIntArgument, std::nullopt, std::reference_wrapper(x), 1);
    CHECK(x == 2);
    MethodPtrUtils::Call((e::system::methodptr)&MethodWithoutArguments, std::nullopt, std::reference_wrapper(x));
    CHECK(x == 123);
    MethodPtrUtils::Call((e::system::methodptr)&MethodWithoutResult, std::nullopt, std::nullopt, 1, (intptr_t)&x);
    CHECK(x == 2);
    MethodPtrUtils::Call((e::system::methodptr)&MethodWithAStringArgument, std::nullopt, std::reference_wrapper(x), EOC_STR_CONST("Hello World"));
    CHECK(x == 11);
    e::system::any y;
    MethodPtrUtils::Call((e::system::methodptr)&MethodWithAStringArgument, std::nullopt, std::reference_wrapper(y), EOC_STR_CONST("Hello World"));
    CHECK(y == e::system::any(11));
}