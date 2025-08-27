/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "EtradePortfolio.hpp"

#include "CsvFile.hpp"

#include <cassert>
#include <fstream>
#include <iostream>

using namespace portopt;

constexpr const char* CASH = "CASH";

EtradePortfolio::EtradePortfolio(const FilePath& csvPath)
{
    std::cerr << "EtradePortfolio::EtradePortfolio [csvPath] " << csvPath << "\n";
    const CsvFile csv { csvPath, false };
    assert(csv.header().empty());

    const auto& data = csv.data();
    assert(data.size() >= 18); // 18 rows minimum, 11 (header) + 1 (asset) + 6 (footer)
    assert(data.at(0).at(0) == "Account Summary");
    assert(data.at(1).at(0) == "Account");
    assert(data.at(1).at(1) == "Total Assets");
    assert(data.at(5).at(0) == "View Summary - All Positions");
    assert(data.at(7).at(0) == "Symbol");
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

        const double lastPrice = std::stod(data.at(i).at(1));
        m_priceInfo.insert({ symbol, lastPrice });

        const double totalValue = quantity * lastPrice;
        std::cerr << "EtradePortfolio::EtradePortfolio [Symbol, Quantity, LastPrice, TotalValue] " << symbol << " : " << quantity << " * $" << lastPrice << " = $" << totalValue << "\n";
    }

    // load cash balance
    const size_t cashRow = data.size() - 6;
    assert(data.at(cashRow).size() == 11);
    assert(data.at(cashRow).at(0) == CASH);

    const double cash = std::stod(data.at(cashRow).at(9));
    m_holdings.insert({ CASH, cash });
    m_priceInfo.insert({ CASH, 1.0 });
    std::cerr << "EtradePortfolio::EtradePortfolio [CASH] $" << cash << "\n";
}

void EtradePortfolio::saveAllocations(const FilePath& csvPath) const
{
    std::cerr << "EtradePortfolio::saveAllocations [csvPath] " << csvPath << "\n";
    std::ofstream file(csvPath);
    if (!file.is_open()) {
        std::cerr << "EtradePortfolio::saveAllocations failed to open file " << csvPath << "\n";
        return;
    }

    double totalPortfolioValue = 0.0;
    for (const auto& [symbol, quantity] : m_holdings) {
        const double lastPrice = m_priceInfo.at(symbol);
        assert(quantity > 0 && lastPrice > 0);
        totalPortfolioValue += quantity * lastPrice;
    }

    file << "Symbol,Quantity,Last Price,Total Value,Allocation\n";
    for (const auto& [symbol, quantity] : m_holdings) {
        const double lastPrice = m_priceInfo.at(symbol);
        const double totalValue = quantity * lastPrice;
        const double allocation = totalValue / totalPortfolioValue * 100.0;
        file << symbol << "," << quantity << "," << lastPrice << "," << totalValue << "," << allocation << "\n";
    }
    file.close();
    std::cerr << "EtradePortfolio::saveAllocations saved to " << csvPath << "\n";
}
