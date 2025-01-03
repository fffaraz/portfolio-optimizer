/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "lib/CsvFile.hpp"
#include "lib/Market.hpp"
#include "lib/Portfolio.hpp"

#include <filesystem>
#include <iostream>
#include <set>

using namespace Farazlib;

int main()
{
    const CsvFile marketInfo { "./data/market.csv", true };
    const std::set<std::string> symbols { "VOO", "VTI" };
    const Market market { "./data/symbols", marketInfo, symbols };

    market.saveAssets("./output/symbols"); // assets' ohlc
    market.saveSymbols("./output/marketSymbols.txt"); // symbols array
    market.saveCorrelationList("./output/CorrelationList.txt");
    market.saveMarketInfo("./output/MarketInfo.csv");

    std::cout << "\nDONE\n";
    return 0;
}
