/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#pragma once

#include "CsvFile.hpp"
#include "OhlcEnums.hpp"

#include <chrono>
#include <string>

namespace Farazlib {

using DateTime = std::chrono::system_clock::time_point;

// Open, High, Low, Close (OHLC) structure
struct Ohlc {
    Ohlc() = default;
    explicit Ohlc(double value) noexcept;
    Ohlc(double open, double high, double low, double close, double volume = 0);
    explicit Ohlc(const CsvFile::RowType& record);

    bool isValid {}; // Whether data was parsed correctly
    bool isFake {}; // Set to true for filling missing data

    DateTime datetime {};

    double open {};
    double high {};
    double low {};
    double close {};

    double volume {};
    double dividends {};
    double splits {};

    double get(PriceType type) const;
    std::string to_string() const;

    double hl2() const noexcept { return (high + low) / 2.0; } ///< midpoint
    double hlc3() const noexcept { return (high + low + close) / 3.0; }
    double ohlc4() const noexcept { return (open + high + low + close) / 4.0; }
};

} // namespace Farazlib
