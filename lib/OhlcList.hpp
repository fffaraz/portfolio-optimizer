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
    [[nodiscard]] size_t size() const noexcept { return m_data.size(); }
    [[nodiscard]] const Ohlc& at(size_t i) const; // first elemet (data[0]) is the most recent
    void save(const std::filesystem::path& filePath) const;

    [[nodiscard]] PriceDirection priceDirection(size_t i, size_t offset) const;
    [[nodiscard]] double priceChange(size_t i) const;
    [[nodiscard]] double priceChange(size_t i, size_t offset, PriceType type) const;

    [[nodiscard]] std::vector<double> toVector(size_t size, size_t offset, PriceType type) const;
    [[nodiscard]] std::vector<double> sma(size_t length, PriceType type) const; // Simple Moving Average
    [[nodiscard]] std::vector<double> wma(size_t length, PriceType type) const; // Weighted Moving Average
    [[nodiscard]] std::vector<double> ema(size_t length, PriceType type) const; // Exponential Moving Average
    [[nodiscard]] std::vector<double> dema(size_t length, PriceType type) const; // Double Exponential Moving Average
    [[nodiscard]] std::vector<double> tema(size_t length, PriceType type) const; // Triple Exponential Moving Average
    // volatility
    // momentum
    // stochasticK
    // williamsR
    // RSI

    [[nodiscard]] double allTimeHigh(size_t skip) const;
    [[nodiscard]] std::vector<double> allTimeHigh() const;

    [[nodiscard]] double percentFromAth(size_t i) const;
    [[nodiscard]] std::vector<double> percentFrom(const std::vector<double>& ath) const;

    [[nodiscard]] double percentToAth(size_t i) const;
    [[nodiscard]] std::vector<double> percentTo(const std::vector<double>& ath) const;

    [[nodiscard]] double avgReturn(size_t length) const;
    [[nodiscard]] double avgRisk(size_t length) const;
    [[nodiscard]] bool matchDatetime(const OhlcList& other, size_t maxSize) const;

private:
    const OhlcVector m_data;
    const OhlcTimeFrame m_timeFrame;
};

} // namespace Farazlib
