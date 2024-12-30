/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#pragma once

#include "CsvFile.hpp"
#include "Ohlc.hpp"

namespace Farazlib {

// List of Open, High, Low, Close (OHLC) data
class OhlcList {
public:
    using OhlcVector = std::vector<Ohlc>;

    explicit OhlcList(double price);
    explicit OhlcList(OhlcVector data);
    OhlcList(const CsvFile& csv, OhlcTimeFrame timeFrame);

    // const TableType& data() const noexcept { return m_data; }
    size_t size() const noexcept { return m_data.size(); }
    const Ohlc& at(size_t i) const; // first elemet (data[0]) is the most recent
    void save(const std::filesystem::path& filePath) const;

    PriceDirection priceDirection(size_t i, size_t offset) const;
    double priceChange(size_t i) const;
    double priceChange(size_t i, size_t offset, PriceType type) const;

    std::vector<double> toVector(size_t size, size_t offset, PriceType type) const;
    std::vector<double> sma(size_t length, PriceType type) const; // Simple Moving Average
    std::vector<double> wma(size_t length, PriceType type) const; // Weighted Moving Average
    std::vector<double> ema(size_t length, PriceType type) const; // Exponential Moving Average
    std::vector<double> dema(size_t length, PriceType type) const; // Double Exponential Moving Average
    std::vector<double> tema(size_t length, PriceType type) const; // Triple Exponential Moving Average
    // volatility
    // momentum
    // stochasticK
    // williamsR
    // RSI

    double allTimeHigh(const size_t skip) const;
    std::vector<double> allTimeHigh() const;

    double percentFromAth(const size_t i) const;
    std::vector<double> percentFrom(const std::vector<double>& ath) const;

    double percentToAth(const size_t i) const;
    std::vector<double> percentTo(const std::vector<double>& ath) const;

    double avgReturn(size_t length) const;
    double avgRisk(size_t length) const;
    bool matchDatetime(const OhlcList& other, size_t maxSize) const;

private:
    const OhlcVector m_data;
    const OhlcTimeFrame m_timeFrame;
};

} // namespace Farazlib
