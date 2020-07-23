#include <catch2/catch.hpp>
#include <e/lib/spec/MMM.h>
using namespace e::lib::spec;
TEST_CASE("MMM Alloc/Free", "[MMM]")
{
    auto p = MMM::Alloc(10, true);
    char buf[10]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    CHECK(memcmp((void *)p, buf, 10) == 0);
    *((uint8_t *)p) = 1;
    buf[0] = 1;
    CHECK(memcmp((void *)p, buf, 10) == 0);
    MMM::Free(p);
}
TEST_CASE("MMM AddressOf", "[MMM]")
{
    int32_t foo = 0;
    CHECK(MMM::AddressOf(foo) == reinterpret_cast<intptr_t>(&foo));
    e::system::any bar(1.0);
    CHECK(MMM::AddressOf(bar) == reinterpret_cast<intptr_t>(bar.address()));
    auto tStr = EOC_STR_CONST("tStr");
    CHECK(MMM::AddressOf(tStr) == reinterpret_cast<intptr_t>(&tStr));
}
TEST_CASE("MMM DataAddressOf", "[MMM]")
{
    int32_t foo = 0;
    CHECK(MMM::DataAddressOf(foo) == reinterpret_cast<intptr_t>(&foo));
    e::system::any bar(1.0);
    CHECK(MMM::DataAddressOf(bar) == reinterpret_cast<intptr_t>(bar.address()));
    auto tStr = EOC_STR_CONST("tStr");
    CHECK(MMM::DataAddressOf(tStr) == reinterpret_cast<intptr_t>(tStr.c_str()));
    e::system::bin tBin{1, 2, 3};
    CHECK(MMM::DataAddressOf(tBin) == reinterpret_cast<intptr_t>(tBin.GetElemPtr()));
    CHECK(MMM::DataAddressOf(e::system::bin{}) == 0);
    e::system::array<int32_t> tArray{1, 2, 3};
    CHECK(MMM::DataAddressOf(tArray) == reinterpret_cast<intptr_t>(tArray.GetElemPtr()));
    e::system::any tBinStoredInAny(tBin);
    CHECK(MMM::DataAddressOf(tBinStoredInAny) == reinterpret_cast<intptr_t>(tBinStoredInAny.cast<e::system::bin>().GetElemPtr()));
    e::system::any tArrayStoredInAny(tArray);
    CHECK(MMM::DataAddressOf(tArrayStoredInAny) == reinterpret_cast<intptr_t>(tArrayStoredInAny.cast<e::system::array<int32_t>>().GetElemPtr()));
}