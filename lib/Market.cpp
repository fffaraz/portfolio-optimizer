/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "Market.hpp"
#include "EnumUtils.hpp"
#include "Utils.hpp"

#include <algorithm> // std::min
#include <filesystem>
#include <fstream>
#include <iomanip> // std::setprecision
#include <iostream>
#include <set>
#include <sstream> // std::stringstream
#include <unordered_map>

using namespace portopt;

namespace {

auto loadAssetInfo(const CsvFile& infoCsv)
{
    std::unordered_map<std::string, CsvFile::RowType> result; // symbol -> asset info
    for (const auto& item : infoCsv.data()) {
        const std::string& symbol = item.at(0);
        result.insert({ symbol, item });
    }
    return result;
}

auto getAssetInfo(const CsvFile::RowType& info)
{
    assert(info.size() == 3);
    AssetInfo result;
    try {
        result.dividendYield = std::stod(info.at(1));
    } catch (...) {
        assert(false);
    }
    try {
        result.expenseRatio = std::stod(info.at(2));
    } catch (...) {
        assert(false);
    }
    return result;
}

auto loadAssetsFromFile(const std::filesystem::path& dataDir, const CsvFile& infoCsv, const std::set<std::string>& symbols)
{
    std::map<std::string, Asset> result;

    // Check if dataDir exists
    if (!std::filesystem::exists(dataDir)) {
        std::cerr << "Market::loadAssetsFromFile [dataDir not found] " << dataDir << "\n";
        assert(false && "dataDir not found");
        return result;
    }

    // load csv file for assets' extra information
    const auto infoMap = loadAssetInfo(infoCsv);

    // for each .csv file in dataDir create and load SYM.csv and SYM.json
    for (const auto& entry : std::filesystem::directory_iterator(dataDir)) {
        auto filename = entry.path().filename();
        if (filename.extension() == ".csv") {
            const auto symbol = filename.replace_extension().string();
            if (!symbols.empty() && !symbols.contains(symbol)) {
                continue; // no need to load this symbol
            }
            if (result.contains(symbol)) {
                std::cerr << "Market::loadAssetsFromFile [duplicate symbol] " << symbol << "\n";
                continue;
            }
            if (infoMap.contains(symbol)) {
                result.insert({ symbol, Asset { symbol, dataDir, getAssetInfo(infoMap.at(symbol)) } });
            } else {
                result.insert({ symbol, Asset { symbol, dataDir, AssetInfo {} } });
            }
        }
    }
    return result;
}

auto loadAssetsFromVector(const std::vector<Asset>& assets)
{
    std::map<std::string, Asset> result;
    for (const auto& item : assets) {
        if (result.contains(item.symbol())) {
            std::cerr << "Market::loadAssetsFromVector [duplicate symbol] " << item.symbol() << "\n";
            continue;
        }
        result.insert({ item.symbol(), item });
    }
    return result;
}

} // anonymous namespace

Market::Market(const std::filesystem::path& symbolsDir, const CsvFile& infoCsv, const std::set<std::string>& symbols)
    : m_assets { loadAssetsFromFile(symbolsDir, infoCsv, symbols) }
{
    std::cerr << "\nMarket::Market assets.size: " << m_assets.size() << "\n";
}

Market::Market(const std::vector<Asset>& assets)
    : m_assets { loadAssetsFromVector(assets) }
{
    std::cerr << "\nMarket::Market assets.size: " << m_assets.size() << "\n";
}

const Asset& Market::get(const std::string& symbol) const
{
    if (!m_assets.contains(symbol)) {
        if (symbol == "CASH") {
            static const Asset cash { symbol, 1, {} };
            return cash;
        }
        std::cerr << "Market::get [Failed] " << symbol << "\n";
        static std::unordered_map<std::string, Asset> unknownAssets;
        if (!unknownAssets.contains(symbol)) {
            unknownAssets.insert({ symbol, { symbol, 0, {} } });
        }
        return unknownAssets.at(symbol);
    }
    return m_assets.at(symbol);
}

double Market::correlation(const std::string& symbol1, const std::string& symbol2) const
{
    const auto& asset1 = get(symbol1);
    const auto& asset2 = get(symbol2);
    return asset1.correlation(asset2, PriceType::HL2, false, 400);
}

void Market::saveAssets(const std::filesystem::path& symbolsDir) const
{
    for (const auto& asset : m_assets) {
        asset.second.save(symbolsDir);
    }
}

void Market::saveCorrelationList(const std::filesystem::path& filePath) const
{
    std::cerr << "\nMarket::saveCorrelationList [begin]\n";
    std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);
    assert(outFile.is_open());

    for (const auto& item1 : m_assets) {
        const auto& asset = item1.second;
        if (!asset.isETF()) {
            continue;
        }
        std::cerr << "Market::saveCorrelationList [sym] " << asset.symbol() << "\n";

        outFile << item1.first
                << " (" << asset.yahoo("longName") << ") [" << asset.info().expenseRatio << "] "
                << asset.tags() << "\n";

        std::vector<std::pair<double, std::string>> list; // list of correlations with other ETFs (correlation, symbol)

        for (const auto& item2 : m_assets) {
            if (item1.first == item2.first || !item2.second.isETF()) {
                continue; // skip the same asset and non-ETF assets
            }

            const auto correlation1 = asset.correlation(item2.second, PriceType::HL2, false, 400);
            const auto correlation2 = asset.correlation(item2.second, PriceType::HL2, true, 400);

            std::stringstream ss;
            ss << std::setprecision(3) << correlation1 << "\t" << correlation2 << "\t"
               << item2.first
               << " (" << item2.second.yahoo("longName") << ") [" << item2.second.info().expenseRatio << "] "
               << item2.second.tags();

            list.emplace_back(correlation1, ss.str());
        }

        std::sort(list.rbegin(), list.rend()); // sort in descending order

        const size_t maxSize = std::min(static_cast<size_t>(10), list.size()); // top 10 with highest correlation
        for (size_t i = 0; i < maxSize; ++i) {
            if (list[i].first > 0.95) {
                outFile << "\t" << list[i].second << "\n";
            }
        }
        outFile << "\n";
    }
    std::cerr << "Market::saveCorrelationList [end]\n";
}

void Market::saveMarketInfo(const std::filesystem::path& filePath) const
{
    std::cerr << "\nMarket::saveMarketInfo [begin]\n";
    std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);
    assert(outFile.is_open());

    // Header   1      2    3        4             5            6              7            8           9            10            11        12
    outFile << "symbol,name,category,dividendYield,expenseRatio,percentFromAth,percentToAth,historySize,return30days,return365days,avgReturn,avgRisk";

    // Tags
    for (AssetTag tag = AssetTag::ETF; tag <= AssetTag::Unclassified; tag = static_cast<AssetTag>(static_cast<int>(tag) + 1)) {
        outFile << ",is-" << EnumUtils::to_string(tag);
    }

    // Pearson Correlations
    for (const auto& item : m_assets) {
        if (!item.second.isETF()) {
            continue;
        }
        outFile << ",PC-" << item.first;
    }

    // Spearman Correlations
    for (const auto& item : m_assets) {
        if (!item.second.isETF()) {
            continue;
        }
        outFile << ",SC-" << item.first;
    }

    outFile << "\n";

    for (const auto& item1 : m_assets) {
        const auto& asset = item1.second;
        std::cerr << "Market::saveMarketInfo [sym] " << asset.symbol() << "\n";

        outFile << item1.first << "," // 1
                << asset.yahoo("longName") << "," // 2
                << asset.yahoo("category") << asset.yahoo("sector") << "," // 3
                << asset.info().dividendYield << "," // 4
                << asset.info().expenseRatio << "," // 5
                << asset.ohlc().percentFromAth(0) << "," // 6
                << asset.ohlc().percentToAth(0) << "," // 7
                << asset.ohlc().size() << "," // 8
                << asset.ohlc().priceChange(0, 30, PriceType::HL2) << "," // 9
                << asset.ohlc().priceChange(0, 365, PriceType::HL2) << "," // 10
                << asset.ohlc().avgReturn(365) << "," // 11
                << asset.ohlc().avgRisk(365); // 12

        // Tags
        for (AssetTag tag = AssetTag::ETF; tag <= AssetTag::Unclassified; tag = static_cast<AssetTag>(static_cast<int>(tag) + 1)) {
            outFile << "," << asset.hasTag(tag);
        }

        // Pearson Correlation
        for (const auto& item2 : m_assets) {
            if (!item2.second.isETF()) {
                continue;
            }
            outFile << "," << item1.second.correlation(item2.second, PriceType::OHLC4, false, 400);
        }

        // Spearman Correlation
        for (const auto& item2 : m_assets) {
            if (!item2.second.isETF()) {
                continue;
            }
            outFile << "," << item1.second.correlation(item2.second, PriceType::OHLC4, true, 400);
        }

        outFile << "\n";
    }
    std::cerr << "Market::saveMarketInfo [end]\n";
}

void Market::saveSymbols(const std::filesystem::path& filePath) const
{
    std::cerr << "\nMarket::saveSymbols\n";
    std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);
    assert(outFile.is_open());

    outFile << "[";
    for (const auto& [symbol, asset] : m_assets) {
        outFile << "\"" << symbol << "\",";
    }
    outFile << "]\n\n";

    for (const auto& [symbol, asset] : m_assets) {
        outFile << symbol << "\t" << asset.yahoo("longName") << "\t" << asset.tags() << "\n";
    }
}
