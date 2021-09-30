/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "lib/portfolio.hpp"

#include <filesystem>
#include <iostream>

using namespace Farazlib;

int main()
{
    std::cout << "current_path: " << std::filesystem::current_path() << std::endl;
    const std::string basePath { "../../portfolio-optimizer/" };

    const CsvFile marketInfo { basePath + "data/market.csv", true };
    const Market market { basePath + "data/symbols", marketInfo };

    market.saveSymbols(basePath + "output/marketSymbols.txt");
    market.saveCorrelationList(basePath + "output/CorrelationList.txt");
    market.saveMarketInfo(basePath + "output/MarketInfo.csv");

    // const CsvFile portfolioCsv { basePath + "data/portfolio.csv", true };
    const CsvFile portfolioCsv { basePath + "data/individual.csv", true };
    const Portfolio portfolio { portfolioCsv, market };

    const double totalValue = portfolio.totalValue(0);
    std::cout << "\nportfolio.totalValue(NOW): " << totalValue << std::endl;

    portfolio.saveSymbols(basePath + "output/portfolioSymbols.txt");
    portfolio.saveAllocations(basePath + "output/Allocations.csv");

    std::cout << "\nDONE" << std::endl;
    return 0;
}
