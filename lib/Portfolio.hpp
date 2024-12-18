/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#pragma once

#include <map>
#include <string>

namespace Farazlib {

class Portfolio {
public:
    Portfolio() = default; // new empty portfolio
    explicit Portfolio(const std::string& csvPath); // load portfolio from csv file
    virtual ~Portfolio() = default;

    void set(const std::string& symbol, double quantity); // add or update symbol quantity
    double get(const std::string& symbol) const; // get quantity of a symbol

    using HoldingsType = std::map<std::string, double>;
    const HoldingsType& holdings() const noexcept; // get all holdings

    void saveCsv(const std::string& csvPath) const; // save portfolio to csv file
    void saveSymbols(const std::string& filePath) const; // save symbols array

protected:
    HoldingsType m_holdings; ///< List of {Symbol -> Quantity} pairs
};

} // namespace Farazlib
