/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

// Efficient frontier for 2 asset portfolio based on individual risk-return
// https://www.portfoliovisualizer.com/asset-correlations

#include "lib/Asset.hpp"
#include "lib/Market.hpp"
#include "lib/Portfolio.hpp"
#include "lib/Utils.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

using namespace Farazlib;

struct __attribute__((aligned(64))) AssetData {
    std::string symbol;
    double avgRisk {};
    double avgReturn {};
};

void calc(std::ofstream& outFile, const int category, const AssetData ad1, const AssetData ad2, const double correlation)
{
    AssetInfo info1;
    info1.avgRisk = ad1.avgRisk;
    info1.avgReturn = ad1.avgReturn;
    info1.correlation[ad2.symbol] = correlation;
    const Asset asset1 { ad1.symbol, 1, info1 };

    AssetInfo info2;
    info2.avgRisk = ad2.avgRisk;
    info2.avgReturn = ad2.avgReturn;
    info2.correlation[ad1.symbol] = correlation;
    const Asset asset2 { ad2.symbol, 1, info2 };

    const Market market { { asset1, asset2 } };

    for (int i = 0; i <= 100; i += 5) {
        Portfolio portfolio {};
        portfolio.set(asset1.symbol(), i);
        portfolio.set(asset2.symbol(), 100 - i);
        outFile << category << ","
                << ad1.symbol << ":" << i << "-" << ad2.symbol << ":" << (100 - i) << ","
                << Utils::avgRisk(market, portfolio) * 100 << "," << Utils::avgReturn(market, portfolio) * 100 << "\n";
    }
}

int main()
{
    const std::string basePath { "../../portfolio-optimizer/" };
    std::ofstream outFile(basePath + "output/simple-risk-return.csv", std::ios::out | std::ios::trunc);
    outFile << "category,portfolio,risk,return\n";

    AssetData bnd { "BND", 0.0323, 0.0317 };
    AssetData voo { "VOO", 0.1318, 0.1562 };
    AssetData sgol { "SGOL", 0.1637, 0.0268 };
    AssetData vnq { "VNQ", 0.1580, 0.1078 };

    calc(outFile, 1, bnd, voo, -0.07);
    calc(outFile, 2, bnd, sgol, 0.28);
    calc(outFile, 3, bnd, vnq, 0.11);
    calc(outFile, 4, voo, sgol, 0.02);
    calc(outFile, 5, voo, vnq, 0.76);
    calc(outFile, 6, sgol, vnq, 0.08);

    std::cout << "\nDONE\n";
    return 0;
}
