/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "lib/CsvFile.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <vector>

using namespace Farazlib;

using Weights = std::map<std::pair<std::string, std::string>, double>; // {from, to} -> rate

namespace {

void print(const Weights& table)
{
    std::cout << "table.size: " << table.size() << "\n";
    for (const auto& item : table) {
        std::cout << item.first.first << " -> " << item.first.second << " : " << item.second << "\n";
    }
    std::cout << "\n";
}

// https://en.wikipedia.org/wiki/Bellman%E2%80%93Ford_algorithm
// https://stackoverflow.com/questions/2282427/interesting-problem-currency-arbitrage

void runBellmanFord(const std::vector<std::string>& assets, const std::vector<std::vector<double>>& rates, size_t source)
{
    const size_t numAssets = assets.size();
    assert(numAssets > 0);
    assert(source < numAssets);

    std::vector<double> distance(numAssets);
    std::vector<int> predecessor(numAssets);

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
                    predecessor[j] = static_cast<int>(i);
                }
            }
        }
    }

    for (size_t k = 0; k < numAssets; ++k) {
        std::cout << "assets[" << k << "] = " << assets[k] << "\n";
        std::cout << "distance[" << k << "] = " << distance[k] << "\n";
        std::cout << "predecessor[" << k << "] = " << predecessor[k] << "\n";
        std::cout << "\n";
    }

    // Step 3: check for negative-weight cycles
    for (size_t i = 0; i < numAssets; ++i) {
        for (size_t j = 0; j < numAssets; ++j) {
            if (distance[i] + rates[i][j] < distance[j]) {
                // Step 4: find a negative-weight cycle
                // Node j is part of a negative cycle
                std::cout << "distance[" << i << "] + " << rates[i][j] << " < distance[" << j << "]\n";
            }
        }
    }
}

template <typename T>
std::vector<T> to_vector(const std::set<T>& set)
{
    std::vector<T> result;
    result.reserve(set.size());
    std::copy(set.begin(), set.end(), std::back_inserter(result));
    return result;
}

} // anonymous namespace

int main()
{
    const CsvFile csvPairs { "./data/misc/pairs.csv", false };
    Weights table; // {from, to} -> rate
    std::set<std::string> assetsSet; // unique assets
    for (const auto& item : csvPairs.data()) {
        auto asset1 = item.at(0); // from
        auto asset2 = item.at(1); // to
        auto rate = std::stod(item.at(2)); // rate
        table.insert({ { asset1, asset2 }, rate });
        assetsSet.insert(asset1);
        assetsSet.insert(asset2);
    }
    print(table);

    const std::vector<std::string> assets = to_vector(assetsSet);
    const size_t numAssets = assets.size();
    assert(numAssets > 0);

    std::vector<std::vector<double>> rates;
    rates.resize(numAssets);

    for (size_t i = 0; i < numAssets; ++i) {
        rates[i].resize(numAssets);
        for (size_t j = 0; j < numAssets; ++j) {
            auto asset1 = assets.at(i);
            auto asset2 = assets.at(j);
            double rate = 0;
            if (table.find({ asset1, asset2 }) != table.end()) {
                rate = table.at({ asset1, asset2 });
            } else if (table.find({ asset2, asset1 }) != table.end()) {
                rate = 1.0 / table.at({ asset2, asset1 });
            } else if (asset1 == asset2) {
                rate = 1;
            } else {
                std::cout << "rate not found: " << asset1 << " -> " << asset2 << "\n";
            }
            rates[i][j] = rate; // rate from asset1 to asset2
        }
    }
    std::cout << "\n";

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "     ";
    for (size_t i = 0; i < numAssets; ++i) {
        std::cout << assets[i] << "   ";
    }
    std::cout << "\n";
    for (size_t i = 0; i < numAssets; ++i) {
        std::cout << assets[i] << "  ";
        for (size_t j = 0; j < numAssets; ++j) {
            std::cout << rates[i][j] << "  ";
            rates[i][j] = -std::log(rates[i][j]); // apply the negative logarithm
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    runBellmanFord(assets, rates, 0);

    return 0;
}
