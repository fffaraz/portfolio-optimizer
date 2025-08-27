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

using namespace portopt;

int main(int argc, char* argv[])
{
    if (argc > 1) {
        if (chdir(argv[1]) != 0) {
            std::cerr << "Error changing directory to " << argv[1] << "\n";
            return 1;
        }
    }

    const CsvFile marketInfo { "./data/misc/market.csv", true };
    const std::set<std::string> symbols; // { "VOO", "VTI" };
    const Market market { "./data/yf", marketInfo, symbols };

    market.saveAssets("./data/output/symbols"); // save ohlc for assets
    market.saveSymbols("./data/output/market-info-symbols.txt"); // symbols array
    market.saveCorrelationList("./data/output/market-info-correlation.txt");
    market.saveMarketInfo("./data/output/market-info.csv");

    std::cout << "\nDONE\n";
    return 0;
}
