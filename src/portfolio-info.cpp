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
    const EtradePortfolio portfolio { "./data/etrade/PortfolioDownload.csv" };

    std::cout << "Number of holdings: " << portfolio.holdings().size() << "\n";
    portfolio.saveCsv("./data/output/portfolio.csv"); // Symbol, Quantity
    portfolio.saveSymbols("./data/output/symbols.txt"); // [sym1, sym2, ...]

    const Portfolio portfolio2 { "./data/output/portfolio.csv" };
    std::cout << "Number of holdings: " << portfolio2.holdings().size() << "\n";
    assert(portfolio.holdings().size() == portfolio2.holdings().size());

    const CsvFile marketInfo { "./data/misc/market.csv", true }; // Symbol, Dividend Yield, Expense Ratio
    const Market market { "./data/yf", marketInfo };

    std::cout << "\n";

    market.saveCorrelationList("./data/output/CorrelationList.txt");
    market.saveMarketInfo("./data/output/MarketInfo.csv");

    std::cout << "\n";

    const double totalValue = Utils::totalValue(market, portfolio);
    std::cout << "\ntotalValue: " << totalValue << "\n";

    Utils::saveAllocations(market, portfolio, "./data/output/Allocations.csv");

    std::cout << "\nDONE\n";

    return 0;
}
