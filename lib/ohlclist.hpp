/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#pragma once

#include "csvfile.hpp"
#include "ohlc.hpp"

namespace Farazlib {

class OhlcList {
public:
    using OhlcVector = std::vector<Ohlc>;

    explicit OhlcList(double price);
    explicit OhlcList(OhlcVector data);
    OhlcList(const CsvFile& csv, OhlcTimeFrame timeFrame);

    // const TableType& data() const noexcept { return m_data; }
    size_t size() const noexcept { return m_data.size(); }
    const Ohlc& at(size_t i) const; // first elemet (data[0]) is the most recent
    void save(const std::string& filePath) const;

    PriceDirection priceDirection(size_t i, size_t offset) const;
    double priceChange(size_t i) const;
    double priceChange(size_t i, size_t offset, PriceType type) const;
    std::vector<double> vector(size_t size, size_t offset, PriceType type) const;
    std::vector<double> sma(size_t length, PriceType type) const;
    std::vector<double> wma(size_t length, PriceType type) const;
    std::vector<double> ema(size_t length, PriceType type) const;
    double allTimeHigh() const;
    double percentFromAth() const;
    double percentToAth() const;
    double avgReturn(size_t length) const;
    double avgRisk(size_t length) const;
    bool matchDatetime(const OhlcList& other, size_t maxSize) const;

private:
    const OhlcVector m_data;
    const OhlcTimeFrame m_timeFrame;
};

} // namespace Farazlib
