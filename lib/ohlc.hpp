/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#pragma once

#include "csvfile.hpp"
#include "ohlcenums.hpp"

#include <QDateTime>

#include <string>

namespace Farazlib {

struct Ohlc {
    // Ohlc() = default;
    explicit Ohlc(const CsvFile::RecordType& record);
    explicit Ohlc(double value) noexcept { open = high = low = close = value; }
    Ohlc(double open, double high, double low, double close, double volume = 0);

    bool isValid {}; // Whether data was parsed correctly
    bool isFake {}; // Set to true for filling missing data

    QDateTime datetime {};

    double open {};
    double high {};
    double low {};
    double close {};

    double volume {};
    double dividends {};
    double splits {};

    double hl2() const noexcept { return (high + low) / 2.0; } ///< midpoint
    double hlc3() const noexcept { return (high + low + close) / 3.0; }
    double ohlc4() const noexcept { return (open + high + low + close) / 4.0; }
    double get(PriceType type) const;

    std::string to_string() const;
};

} // namespace Farazlib
