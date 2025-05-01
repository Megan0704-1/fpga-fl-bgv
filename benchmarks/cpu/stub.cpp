#include "constants.hpp"   
#include "data_type.hpp"

namespace hls {

int remainder(int x, int y)
{
    int r = x % y;
    if (r < 0) r += y;
    return r;
}

data_t remainder(data_t x, data_t y)
{
    data_t r = x % y;
    if (r < (data_t)0) r += y;
    return r;
}

} // namespace hls
