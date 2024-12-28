/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "Portfolio.hpp"

#include "CsvFile.hpp"

#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>

using namespace Farazlib;

Portfolio::Portfolio(const std::string& csvPath)
{
    std::cerr << "Portfolio::Portfolio [csvPath] " << csvPath << "\n";
    const CsvFile csv { csvPath, true };
    const auto& header = csv.header();
    assert(header.size() == 2);
    assert(header.at(0) == "Symbol");
    assert(header.at(1) == "Quantity");
    const auto& data = csv.data();
    for (const auto& row : data) {
        assert(row.size() == 2);
        const std::string& symbol = row.at(0);
        const double quantity = std::stod(row.at(1));
        m_holdings.insert({ symbol, quantity });
    }
}

void Portfolio::set(const std::string& symbol, double quantity)
{
    m_holdings[symbol] = quantity;
}

double Portfolio::get(const std::string& symbol) const
{
    const auto itr = m_holdings.find(symbol);
    return itr == m_holdings.end() ? 0 : itr->second;
}

const Portfolio::HoldingsType& Portfolio::holdings() const noexcept
{
    return m_holdings;
}

void Portfolio::saveCsv(const std::string& csvPath) const
{
    std::cout << "Portfolio::saveCsv [csvPath] " << csvPath << "\n";
    std::ofstream file(csvPath, std::ios::out | std::ios::trunc);
    assert(file.is_open());
    file << "Symbol,Quantity\n";
    for (const auto& [symbol, quantity] : m_holdings) {
        file << symbol << "," << quantity << "\n";
    }
}

void Portfolio::saveSymbols(const std::string& filePath) const
{
    std::cout << "Portfolio::saveSymbols [filePath] " << filePath << "\n";
    std::ofstream file(filePath, std::ios::out | std::ios::trunc);
    assert(file.is_open());
    file << "[";
    for (const auto& [symbol, quantity] : m_holdings) {
        if (symbol == "CASH") {
            continue;
        }
        file << "\"" << symbol << "\", ";
    }
    file << "]\n";
}
