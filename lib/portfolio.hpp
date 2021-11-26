/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#pragma once

#include "csvfile.hpp"
#include "market.hpp"

namespace Farazlib {

class Portfolio {
public:
    using HoldingsType = std::unordered_map<std::string, double>; ///< Map of holdings {Symbol -> Quantity}

    Portfolio(const CsvFile& csv, const Market& market);
    Portfolio(HoldingsType holdings, const Market& market);

    const HoldingsType& holdings() const noexcept { return m_holdings; }

    double totalValue(size_t i = 0) const;
    std::pair<double, std::set<std::string>> totalValue(AssetTag tag) const;
    double valueChange(size_t i, size_t offset) const;

    double avgReturn() const;
    double avgRisk() const;
    double avgReturn(int length) const;
    double avgRisk(int length) const;

    void saveAllocations(const std::string& filePath) const;
    void saveSymbols(const std::string& filePath) const;

private:
    const HoldingsType m_holdings; ///< List of {Symbol, Quantity} pairs
    const Market& m_market; ///< Market information
};

} // namespace Farazlib
