/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

// Efficient frontier for 2 asset portfolio from historical prices

#include "lib/Market.hpp"
#include "lib/Portfolio.hpp"
#include "lib/Utils.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

using namespace Farazlib;

void calc(std::ofstream& outFile, const Market& market, const int category, const std::string symbol1, const std::string symbol2)
{
    const auto total = 1'000'000;
    const auto length = 365 * 15;
    for (int i = 0; i <= 100; i += 5) {
        const auto price1 = market.get(symbol1).ohlc().at(length).hl2();
        const auto price2 = market.get(symbol2).ohlc().at(length).hl2();
        const auto shares1 = i * total / price1 / 100;
        const auto shares2 = (100 - i) * total / price2 / 100;
        Portfolio portfolio {};
        portfolio.set(symbol1, shares1);
        portfolio.set(symbol2, shares2);
        outFile << category << ","
                << symbol1 << ":" << i << "-" << symbol2 << ":" << (100 - i) << ","
                << Utils::avgRisk(market, portfolio, length) * 100 << "," << Utils::avgReturn(market, portfolio, length) * 100 << "\n";
    }
}

int main()
{
    const CsvFile marketInfo { "./data/misc/market.csv", true };
    const std::set<std::string> symbols { "BND", "VOO", "SGOL", "VNQ" };
    const Market market { "./data/yf", marketInfo, symbols };

    std::ofstream outFile("./data/output/two-asset-optimizer.csv", std::ios::out | std::ios::trunc);
    outFile << "category,portfolio,risk,return\n";

    calc(outFile, market, 1, "BND", "VOO");
    calc(outFile, market, 2, "BND", "SGOL");
    calc(outFile, market, 3, "BND", "VNQ");
    calc(outFile, market, 4, "VOO", "SGOL");
    calc(outFile, market, 5, "VOO", "VNQ");
    calc(outFile, market, 6, "SGOL", "VNQ");

    std::cout << "\nDONE\n";
    return 0;
}
