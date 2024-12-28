/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "EtradePortfolio.hpp"

#include "CsvFile.hpp"

#include <cassert>
#include <iostream>

using namespace Farazlib;

constexpr const char* CASH = "CASH";

EtradePortfolio::EtradePortfolio(const std::string& csvPath)
{
    std::cerr << "EtradePortfolio::EtradePortfolio [csvPath] " << csvPath << "\n";
    const CsvFile csv { csvPath, false };
    assert(csv.header().empty());
    const auto& data = csv.data();
    assert(data.size() >= 18); // 18 rows minimum, 11 (header) + 1 (asset) + 6 (footer)
    assert(data.at(0).at(0) == "Account Summary");
    assert(data.at(1).at(0) == "Account");
    assert(data.at(5).at(0) == "View Summary - All Positions");
    assert(data.at(10).at(0) == "Symbol");
    assert(data.at(10).at(1) == "Last Price $");

    // load assets
    for (size_t i = 11; i < data.size() - 6; ++i) {
        assert(data.at(i).size() == 10);
        const std::string& symbol = data.at(i).at(0);
        if (symbol.find(' ') != std::string::npos) {
            continue; // skip options
        }
        const double quantity = std::stod(data.at(i).at(4));
        m_holdings.insert({ symbol, quantity });
        std::cerr << "EtradePortfolio::EtradePortfolio [Symbol, Quantity] " << symbol << " " << quantity << "\n";
    }

    // load cash balance
    const size_t cashRow = data.size() - 6;
    assert(data.at(cashRow).size() == 11);
    assert(data.at(cashRow).at(0) == CASH);
    const double cash = std::stod(data.at(cashRow).at(9));
    m_holdings.insert({ CASH, cash });
    std::cerr << "EtradePortfolio::EtradePortfolio [CASH] " << cash << "\n";
}
