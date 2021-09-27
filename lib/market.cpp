/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "market.hpp"
#include "enumutils.hpp"
#include "utils.hpp"

#include <algorithm> // std::min
#include <filesystem>
#include <fstream>
#include <iomanip> // std::setprecision
#include <iostream>
#include <set>
#include <sstream> // std::stringstream
#include <unordered_map>

using namespace Farazlib;

namespace {

auto loadAssetInfo(const CsvFile& infoCsv)
{
    std::map<std::string, std::vector<std::string>> result;
    for (const auto& item : infoCsv.data()) {
        const std::string symbol = item.at(0);
        result.insert({ symbol, item });
    }
    return result;
}

auto loadAssets(const std::string& dataDir, const CsvFile& infoCsv, const std::set<std::string>& symbols)
{
    Market::AssetsDb result;
    if (!std::filesystem::exists(dataDir)) {
        std::cout << "Market::loadAssets [dataDir not found] " << dataDir << std::endl;
        return result;
    }

    const auto infoMap = loadAssetInfo(infoCsv);
    for (const auto& entry : std::filesystem::directory_iterator(dataDir)) {
        auto filename = entry.path().filename();
        if (filename.extension() == ".csv") {
            const auto symbol = filename.replace_extension().string();
            if (!symbols.empty() && !symbols.contains(symbol)) {
                continue; // no need to load this symbol
            }
            if (result.contains(symbol)) {
                std::cout << "Market::loadAssets [duplicate symbol] " << symbol << std::endl;
                continue;
            }
            const auto info = infoMap.contains(symbol) ? infoMap.at(symbol) : CsvFile::RecordType {};
            result.insert({ symbol, Asset { symbol, dataDir, AssetInfo { info } } });
        }
    }
    return result;
}

auto convertAssets(const std::vector<Asset>& assets)
{
    Market::AssetsDb result;
    for (const auto& item : assets) {
        if (result.contains(item.symbol())) {
            std::cout << "Market::convertAssets [duplicate symbol] " << item.symbol() << std::endl;
            continue;
        }
        result.insert({ item.symbol(), item });
    }
    return result;
}

} // anonymous namespace

Market::Market(const std::string& symbolsDir, const CsvFile& infoCsv, const std::set<std::string>& symbols)
    : m_assets { loadAssets(symbolsDir, infoCsv, symbols) }
{
    std::cout << "Market::Market " << m_assets.size() << std::endl;
}

Market::Market(const std::vector<Asset>& assets)
    : m_assets { convertAssets(assets) }
{
    std::cout << "Market::Market " << m_assets.size() << std::endl;
}

const Asset& Market::get(const std::string& symbol) const
{
    if (!m_assets.contains(symbol)) {
        if (symbol == "CASH") {
            static const Asset cash { symbol, 1, {} };
            return cash;
        }
        std::cout << "Market::get [Failed] " << symbol << std::endl;
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
    return asset1.correlation(asset2, PriceType::HL2, false);
}

void Market::save(const std::string& symbolsDir) const
{
    for (const auto& asset : m_assets) {
        asset.second.save(symbolsDir);
    }
}

void Market::saveCorrelationList(const std::string& filePath) const
{
    std::cout << "Market::saveCorrelationList [begin]" << std::endl;
    std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);
    if (!outFile.is_open()) {
        std::cout << "Market::saveCorrelationList [FAILED TO OPEN FILE]" << std::endl;
    }

    for (const auto& item1 : m_assets) {
        const auto& asset = item1.second;
        if (!asset.isETF()) {
            continue;
        }
        std::cout << "Market::saveCorrelationList [sym] " << asset.symbol() << std::endl;

        outFile << item1.first
                << " (" << asset.yahoo("longName") << ") [" << asset.info().expenseRatio << "] "
                << asset.tags() << std::endl;

        std::vector<std::pair<double, std::string>> list;

        for (const auto& item2 : m_assets) {
            if (item1.first == item2.first || !item2.second.isETF()) {
                continue;
            }

            const auto correlation1 = asset.correlation(item2.second, PriceType::HL2, false);
            const auto correlation2 = asset.correlation(item2.second, PriceType::HL2, true);

            std::stringstream ss;
            ss << std::setprecision(3) << correlation1 << "\t" << correlation2 << "\t"
               << item2.first
               << " (" << item2.second.yahoo("longName") << ") [" << item2.second.info().expenseRatio << "] "
               << item2.second.tags();

            list.push_back({ correlation1, ss.str() });
        }

        sort(list.rbegin(), list.rend());

        const size_t maxSize = std::min(static_cast<size_t>(10), list.size());
        for (size_t i = 0; i < maxSize; ++i) {
            if (list[i].first > 0.95) {
                outFile << "\t" << list[i].second << std::endl;
            }
        }
        outFile << std::endl;
    }
    std::cout << "Market::saveCorrelationList [end]" << std::endl;
}

void Market::saveMarketInfo(const std::string& filePath) const
{
    std::cout << "Market::saveMarketInfo [begin]" << std::endl;
    std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);
    if (!outFile.is_open()) {
        std::cout << "Market::saveMarketInfo [FAILED TO OPEN FILE]" << std::endl;
    }

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

    outFile << std::endl;

    for (const auto& item1 : m_assets) {
        const auto& asset = item1.second;
        std::cout << "Market::saveMarketInfo [sym] " << asset.symbol() << std::endl;

        outFile << item1.first << "," // 1
                << asset.yahoo("longName") << "," // 2
                << asset.yahoo("category") << asset.yahoo("sector") << "," // 3
                << asset.info().dividendYield << "," // 4
                << asset.info().expenseRatio << "," // 5
                << asset.ohlc().percentFromAth() << "," // 6
                << asset.ohlc().percentToAth() << "," // 7
                << asset.ohlc().size() << "," // 8
                << asset.ohlc().priceChange(0, 30) << "," // 9
                << asset.ohlc().priceChange(0, 365) << "," // 10
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
            outFile << "," << item1.second.correlation(item2.second, PriceType::OHLC4, false);
        }

        // Spearman Correlation
        for (const auto& item2 : m_assets) {
            if (!item2.second.isETF()) {
                continue;
            }
            outFile << "," << item1.second.correlation(item2.second, PriceType::OHLC4, true);
        }

        outFile << std::endl;
    }
    std::cout << "Market::saveMarketInfo [end]" << std::endl;
}

void Market::saveSymbols(const std::string& filePath) const
{
    std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);
    if (!outFile.is_open()) {
        std::cout << "Market::saveSymbols [FAILED TO OPEN FILE]" << std::endl;
    }

    outFile << "[";
    for (const auto& [symbol, asset] : m_assets) {
        outFile << "\"" << symbol << "\",";
    }
    outFile << "]" << std::endl
            << std::endl;

    for (const auto& [symbol, asset] : m_assets) {
        outFile << symbol << "\t" << asset.yahoo("longName") << "\t" << asset.tags() << std::endl;
    }
}
