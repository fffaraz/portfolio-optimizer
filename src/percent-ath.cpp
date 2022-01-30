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
    const std::set<std::string> symbols { "AMD", "VOO", "VTI" };
    const Market market { basePath + "data/symbols", marketInfo, symbols };

    market.saveAssets(basePath + "output/symbols"); // assets' ohlc

    std::cout << "\nDONE" << std::endl;
    return 0;
}
