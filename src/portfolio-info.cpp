/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "lib/CsvFile.hpp"
#include "lib/EtradePortfolio.hpp"
#include "lib/Market.hpp"
#include "lib/Utils.hpp"

#include <filesystem>
#include <iostream>

using namespace Farazlib;

int main()
{
    std::cerr << "current_path: " << std::filesystem::current_path().string() << std::endl;
    const std::string basePath { "../../portfolio-optimizer/" };

    const std::string csvPath = basePath + "data/etrade/PortfolioDownload.csv";
    const EtradePortfolio portfolio { csvPath };

    std::cout << "Number of holdings: " << portfolio.holdings().size() << std::endl;
    portfolio.saveCsv(basePath + "data/output/portfolio.csv");
    portfolio.saveSymbols(basePath + "data/output/symbols.txt");

    const Portfolio portfolio2 { basePath + "data/output/portfolio.csv" };
    std::cout << "Number of holdings: " << portfolio2.holdings().size() << std::endl;

    const CsvFile marketInfo { basePath + "data/market.csv", true };
    const Market market { basePath + "data/symbols", marketInfo };

    std::cout << std::endl;

    market.saveCorrelationList(basePath + "output/CorrelationList.txt");
    market.saveMarketInfo(basePath + "output/MarketInfo.csv");

    std::cout << std::endl;

    const double totalValue = Utils::totalValue(market, portfolio);
    std::cout << "\ntotalValue: " << totalValue << std::endl;

    Utils::saveAllocations(market, portfolio, basePath + "output/Allocations.csv");

    std::cout << "\nDONE" << std::endl;

    return 0;
}
