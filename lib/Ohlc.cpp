/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "Ohlc.hpp"
#include "Utils.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>

using namespace Farazlib;

Ohlc::Ohlc(double value) noexcept
    : open { value }
    , high { value }
    , low { value }
    , close { value }
{
}

Ohlc::Ohlc(double open, double high, double low, double close, double volume)
    : open { open }
    , high { high }
    , low { low }
    , close { close }
    , volume { volume }
{
}

Ohlc::Ohlc(const CsvFile::RowType& record)
{
    assert(record.size() == 8);
    datetime = Utils::toDateTime(record.at(0)); // Date
    try {
        open = std::stod(record.at(1)); // Open
        high = std::stod(record.at(2)); // High
        low = std::stod(record.at(3)); // Low
        close = std::stod(record.at(4)); // Close
        volume = std::stod(record.at(5)); // Volume
        dividends = std::stod(record.at(6)); // Dividends
        splits = std::stod(record.at(7)); // Stock Splits
    } catch (std::exception& e) {
        std::cout << "Ohlc::Ohlc [exception] " << e.what() << "\t" << Utils::join(record, ",") << "\n";
        return;
    }

    if (open == 0) { // missing open
        open = hl2();
    }

    assert(open > 0);
    assert(high > 0);
    assert(low > 0);
    assert(close > 0);
    assert(volume >= 0);
    assert(dividends >= 0);
    assert(splits >= 0);

    high = std::max({ open, high, low, close });
    low = std::min({ open, high, low, close });

    assert(high >= open);
    assert(high >= low);
    assert(high >= close);
    assert(low <= open);
    assert(low <= high);
    assert(low <= close);

    isValid = true;
}

double Ohlc::get(PriceType type) const
{
    switch (type) {
    case PriceType::Open:
        return open;
    case PriceType::High:
        return high;
    case PriceType::Low:
        return low;
    case PriceType::Close:
        return close;
    case PriceType::HL2:
        return hl2();
    case PriceType::HLC3:
        return hlc3();
    case PriceType::OHLC4:
        return ohlc4();
    default:
        return 0;
    }
}

std::string Ohlc::to_string() const
{
    // clang-format off
    return Utils::to_string(datetime) +
           "\tO " + std::to_string(open) +
           "\tH " + std::to_string(high) +
           "\tL " + std::to_string(low) +
           "\tC " + std::to_string(close);
           /*
           "\tV " + std::to_string(volume) +
           "\tD " + std::to_string(dividends) +
           "\tS " + std::to_string(splits);
           */
    // clang-format on
}
