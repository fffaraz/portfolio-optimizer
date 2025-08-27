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

using namespace portopt;

int main()
{
    const EtradePortfolio portfolio { "./data/etrade/PortfolioDownload.csv" };

    std::cout << "Number of holdings: " << portfolio.holdings().size() << "\n";
    portfolio.saveCsv("./data/output/portfolio-info-simple.csv"); // Symbol, Quantity
    portfolio.saveAllocations("./data/output/portfolio-info-allocations.csv"); // Symbol, Quantity, Last Price, Total Value, Allocation
    portfolio.saveSymbols("./data/output/portfolio-info-symbols.txt"); // [sym1, sym2, ...]

    const Portfolio portfolio2 { "./data/output/portfolio-info-simple.csv" };
    std::cout << "Number of holdings: " << portfolio2.holdings().size() << "\n";
    assert(portfolio.holdings().size() == portfolio2.holdings().size());

    const CsvFile marketInfo { "./data/misc/market.csv", true }; // Symbol, Dividend Yield, Expense Ratio
    const Market market { "./data/yf", marketInfo };

    std::cout << "\n";

    market.saveCorrelationList("./data/output/portfolio-info-correlation.txt");
    market.saveMarketInfo("./data/output/portfolio-info-market.csv");

    std::cout << "\n";

    const double totalValue = Utils::totalValue(market, portfolio);
    std::cout << "\ntotalValue: " << totalValue << "\n";

    Utils::saveAllocations(market, portfolio, "./data/output/portfolio-info-market-allocations.csv");

    std::cout << "\nDONE\n";

    return 0;
}
