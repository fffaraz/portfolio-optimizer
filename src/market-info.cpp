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
    std::cout << "current_path: " << std::filesystem::current_path() << std::endl;
    const std::string basePath { "../../portfolio-optimizer/" };

    const CsvFile marketInfo { basePath + "data/market.csv", true };
    const std::set<std::string> symbols { "VOO", "VTI" };
    const Market market { basePath + "data/symbols", marketInfo, symbols };

    market.saveAssets(basePath + "output/symbols"); // assets' ohlc
    market.saveSymbols(basePath + "output/marketSymbols.txt"); // symbols array
    market.saveCorrelationList(basePath + "output/CorrelationList.txt");
    market.saveMarketInfo(basePath + "output/MarketInfo.csv");

    std::cout << "\nDONE" << std::endl;
    return 0;
}
