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

    std::cout << std::endl;

    market.saveSymbols(basePath + "output/marketSymbols.txt");
    market.saveCorrelationList(basePath + "output/CorrelationList.txt");
    market.saveMarketInfo(basePath + "output/MarketInfo.csv");

    std::cout << std::endl;

    // const CsvFile portfolioCsv { basePath + "data/portfolio.csv", true };
    const CsvFile portfolioCsv { basePath + "data/etrade/individual.csv", true };
    const Portfolio portfolio { portfolioCsv, market };

    std::cout << std::endl;

    const double totalValue = portfolio.totalValue();
    std::cout << "\nportfolio.totalValue: " << totalValue << std::endl;

    portfolio.saveSymbols(basePath + "output/portfolioSymbols.txt");
    portfolio.saveAllocations(basePath + "output/Allocations.csv");

    std::cout << "\nDONE" << std::endl;
    return 0;
}
