/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

// Efficient frontier for multi asset portfolio loaded from assets.csv
// https://www.portfoliovisualizer.com/asset-correlations

#include "lib/Asset.hpp"
#include "lib/CsvFile.hpp"
#include "lib/Market.hpp"
#include "lib/Portfolio.hpp"
#include "lib/Utils.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace Farazlib;

int main()
{
    const CsvFile assetsCsv { "./data/misc/assets.csv", true };
    const auto& csvHeader = assetsCsv.header();
    std::vector<Asset> assets;

    for (const auto& item : assetsCsv.data()) {
        AssetInfo info;
        const std::string& symbol = item.at(0);
        info.avgRisk = std::stod(item.at(1)) / 100.0;
        info.avgReturn = std::stod(item.at(2)) / 100.0;
        for (size_t i = 3; i < csvHeader.size(); ++i) {
            info.correlation[csvHeader.at(i)] = std::stod(item.at(i));
        }
        assets.emplace_back(symbol, 1, info);
    }

    const Market market { assets };

    // risk -> {return, portfolio}
    std::unordered_map<int, std::pair<double, std::string>> data;

    const size_t iterations = Utils::powi(10, assets.size());
    for (size_t i = 1; i < iterations; ++i) {
        if (i % 2000 == 0) {
            std::cout << 100.0 * i / iterations << "\n";
        }

        Portfolio portfolio {};
        int totalShares {};
        for (size_t j = 0; j < assets.size(); ++j) {
            int shares = (i / Utils::powi(10, j)) % 10;
            shares *= 10;
            totalShares += shares;
            portfolio.set(assets.at(j).symbol(), shares);
        }

        const int avgRisk = std::round(Utils::avgRisk(market, portfolio) * 1000);
        const double avgReturn = Utils::avgReturn(market, portfolio) * 100;

        std::stringstream ss;
        for (const auto& item : assets) {
            ss << item.symbol() << ":" << portfolio.get(item.symbol()) << "-";
        }
        ss << "," << Utils::avgRisk(market, portfolio) * 100 << "," << avgReturn;
        for (const auto& item : assets) {
            ss << "," << 100.0 * portfolio.get(item.symbol()) / totalShares;
        }

        data.insert_or_assign(avgRisk, std::make_pair(avgReturn, ss.str()));
    }

    std::ofstream outFile("./data/output/risk-return-optimizer.csv", std::ios::out | std::ios::trunc);
    outFile << "portfolio,risk,return";
    for (const auto& item : assets) {
        outFile << "," << item.symbol();
    }
    outFile << "\n";

    for (const auto& item : data) {
        outFile << item.second.second << "\n";
    }

    std::cout << "\nDONE\n";
    return 0;
}
