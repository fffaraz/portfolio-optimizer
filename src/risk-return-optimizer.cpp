/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

// Efficient frontier for multi asset portfolio loaded from assets.csv
// https://www.portfoliovisualizer.com/asset-correlations

#include "lib/portfolio.hpp"
#include "lib/utils.hpp"

#include <QRandomGenerator>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace Farazlib;

int main()
{
    std::cout << "current_path: " << std::filesystem::current_path() << std::endl;
    const std::string basePath { "../../portfolio-optimizer/" };

    const CsvFile assetsCsv { basePath + "data/assets.csv", true };
    const auto& csvHeader = assetsCsv.header();
    std::vector<Asset> assets;

    for (const auto& item : assetsCsv.data()) {
        AssetInfo info;
        const std::string symbol = item.at(0);
        info.description = item.at(1);
        info.avgRisk = std::stod(item.at(2)) / 100.0;
        info.avgReturn = std::stod(item.at(3)) / 100.0;
        for (size_t i = 4; i < csvHeader.size(); ++i) {
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
            std::cout << 100.0 * i / iterations << std::endl;
        }

        Portfolio::HoldingsType holdings;
        int totalShares {};
        for (size_t j = 0; j < assets.size(); ++j) {
            int shares = (i / Utils::powi(10, j)) % 10;
            shares *= 10;
            totalShares += shares;
            holdings.insert({ assets.at(j).symbol(), shares });
        }

        const Portfolio portfolio { holdings, market };
        const int avgRisk = std::round(portfolio.avgRisk() * 1000);
        const double avgReturn = portfolio.avgReturn() * 100;

        std::stringstream ss;
        for (const auto& item : assets) {
            ss << item.symbol() << ":" << holdings.at(item.symbol()) << "-";
        }
        ss << "," << portfolio.avgRisk() * 100 << "," << avgReturn;
        for (const auto& item : assets) {
            ss << "," << 100.0 * holdings.at(item.symbol()) / totalShares;
        }

        if (!data.contains(avgRisk)) {
            data.insert({ avgRisk, { avgReturn, ss.str() } });
        } else if (data.at(avgRisk).first < avgReturn) {
            data.at(avgRisk) = { avgReturn, ss.str() };
        }
    }

    std::ofstream outFile(basePath + "output/risk-return-optimizer.csv", std::ios::out | std::ios::trunc);
    outFile << "portfolio,risk,return";
    for (const auto& item : assets) {
        outFile << "," << item.symbol();
    }
    outFile << "\n";

    for (const auto& item : data) {
        outFile << item.second.second << "\n";
    }

    std::cout << "\nDONE" << std::endl;
    return 0;
}
