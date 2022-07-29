/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "lib/portfolio.hpp"

#include <filesystem>
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <map>
#include <limits>

using namespace Farazlib;

typedef std::map<std::pair<std::string, std::string>, double> Weights;

void print(const Weights &table)
{
    std::cout << "table.size: " << table.size() << std::endl;
    for (const auto& item : table) {
        std::cout << item.first.first << " -> " << item.first.second << " : " << item.second << std::endl;
    }
}

// https://en.wikipedia.org/wiki/Bellman%E2%80%93Ford_algorithm
// https://stackoverflow.com/questions/2282427/interesting-problem-currency-arbitrage

void BellmanFord(const std::vector<std::string> &assets, const std::vector<std::vector<double>> &rates, size_t source)
{
    const size_t numAssets = assets.size();
    double distance[numAssets], predecessor[numAssets];

    // Step 1: initialize graph
    for (size_t i = 0; i < numAssets; ++i) {
        distance[i] = std::numeric_limits<double>::max(); // infinity
        predecessor[i] = -1; // null
    }

    distance[source] = 0; // The distance from the source to itself is, of course, zero

    // Step 2: relax edges repeatedly
    for (size_t k = 0; k < numAssets; ++k) {
        for (size_t i = 0; i < numAssets; ++i) {
            for (size_t j = 0; j < numAssets; ++j) {
                if (distance[i] + rates[i][j] < distance[j]) {
                    distance[j] = distance[i] + rates[i][j];
                    predecessor[j] = i;
                }
            }
        }
    }

    for (size_t k = 0; k < numAssets; ++k) {
        std::cout << "assets[" << k << "] = " << assets[k] << std::endl;
        std::cout << "distance[" << k << "] = " << distance[k] << std::endl;
        std::cout << "predecessor[" << k << "] = " << predecessor[k] << std::endl;
        std::cout << std::endl;
    }

    // Step 3: check for negative-weight cycles
    for (size_t i = 0; i < numAssets; ++i) {
        for (size_t j = 0; j < numAssets; ++j) {
            if (distance[i] + rates[i][j] < distance[j]) {
                // Step 4: find a negative-weight cycle
                // Node j is part of a negative cycle
                std::cout << "distance[" << i << "] + " << rates[i][j] << " < distance[" << j << "]" << std::endl;
            }
        }
    }
}

int main()
{
    std::cout << "current_path: " << std::filesystem::current_path() << std::endl;
    const std::string basePath { "../../portfolio-optimizer/" };

    const CsvFile csvPairs { basePath + "data/pairs.csv", false };
    Weights table;
    std::set<std::string> assetsSet;
    for (const auto& item : csvPairs.data()) {
        auto asset1 = item.at(0);
        auto asset2 = item.at(1);
        auto rate = std::stod(item.at(2));
        table.insert({{asset1, asset2}, rate});
        assetsSet.insert(asset1);
        assetsSet.insert(asset2);
    }
    print(table);

    std::vector<std::string> assets;
    assets.reserve(assetsSet.size());
    std::copy(assetsSet.begin(), assetsSet.end(), std::back_inserter(assets));

    const size_t numAssets = assets.size();
    // double rates[numAssets][numAssets];
    std::vector<std::vector<double>> rates;
    rates.resize(numAssets);
    for (size_t i = 0; i < numAssets; ++i) {
        rates[i].resize(numAssets);
        for (size_t j = 0; j < numAssets; ++j) {
            auto asset1 = assets.at(i);
            auto asset2 = assets.at(j);
            double rate = 0;
            if (table.find({asset1, asset2}) != table.end()) {
                rate = table.at({asset1, asset2});
            } else if (table.find({asset2, asset1}) != table.end()) {
                rate = 1.0 / table.at({asset2, asset1});
            } else if (asset1 == asset2) {
                rate = 1;
            } else {
                std::cout << "rate not found: " << asset1 << " -> " << asset2 << std::endl;
            }
            rates[i][j] = rate;
        }
    }

    std::cout << std::endl;
    for (size_t i = 0; i < numAssets; ++i) {
        for (size_t j = 0; j < numAssets; ++j) {
            std::cout << rates[i][j] << "  ";
            rates[i][j] = -log(rates[i][j]);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    BellmanFord(assets, rates, 0);

    return 0;
}
