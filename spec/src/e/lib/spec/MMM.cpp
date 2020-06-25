#include "MMM.h"
#include "stdlib.h"

intptr_t e::lib::spec::MMM::Alloc(intptr_t size, std::optional<bool> fillWithZero)
{
    auto realSize = static_cast<size_t>(size);
    auto pReal = malloc(realSize);
    if(fillWithZero.value_or(false))
    {
        memset(pReal, 0, realSize);
    }
    return reinterpret_cast<intptr_t>(pReal);
}

void e::lib::spec::MMM::Free(intptr_t ptr)
{
    free(reinterpret_cast<void *>(ptr));
}
