/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#pragma once

#include <cstdint>

namespace Farazlib {

// clang-format off
enum class PriceDirection : std::uint8_t {
    VeryUp,   // H2 > H1, L2 > H1
    Up,       // H2 > H1, L2 > L1
    Widen,    // H2 > H1, L2 < L1
    Narrow,   // H2 > L1, L2 > L1
    Down,     // H2 > L1, L2 < L1
    VeryDown, // H2 < L1, L2 < L1
};
// clang-format on

enum class PriceType : std::uint8_t {
    Open,
    High,
    Low,
    Close,
    HL2,
    HLC3,
    OHLC4,
};

enum class OhlcTimeFrame : std::uint8_t {
    Hourly,
    Daily,
    Weekly,
};

} // namespace Farazlib
