/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "portfolio.hpp"
#include "enumutils.hpp"
#include "utils.hpp"

#include <cmath>
#include <fstream>
#include <iostream>

using namespace Farazlib;

namespace {

auto loadSimplePortfolio(const CsvFile::TableType& data)
{
    Portfolio::HoldingsType result;
    for (const auto& item : data) {
        if (item.size() != 2) {
            std::cout << "Portfolio::loadSimplePortfolio [item.size() != 2] " << item.size() << std::endl;
            continue;
        }
        const std::string& symbol = item.at(0);
        const double quantity = std::stod(item.at(1));
        result.insert({ symbol, quantity });
    }
    return result;
}

auto loadETradePortfolio(const CsvFile::TableType& data)
{
    Portfolio::HoldingsType result;
    if (data.size() < 16) {
        std::cout << "Portfolio::loadETradePortfolio [data.size() < 16] " << data.size() << std::endl;
        return result;
    }
    for (size_t i = 10; i < data.size() - 5; ++i) {
        if (data.at(i).size() != 10) {
            std::cout << "Portfolio::loadETradePortfolio [data.at(i).size() != 10] " << data.at(i).size() << std::endl;
            continue;
        }
        const std::string& symbol = data.at(i).at(0);
        double quantity {};
        if (symbol == "CASH") {
            quantity = std::stod(data.at(i).at(9));
        } else {
            quantity = std::stod(data.at(i).at(4));
        }
        result.insert({ symbol, quantity });
        std::cout << "Portfolio::loadETradePortfolio [symbol] " << symbol << "\t" << quantity << std::endl;
    }
    return result;
}

auto loadHoldings(const CsvFile& csv)
{
    const auto& header = csv.header();
    const auto& data = csv.data();
    if (header.size() == 2) { // Symbol, Quantity
        assert(header.at(0) == "Symbol");
        assert(header.at(1) == "Quantity");
        assert(data.size() > 0);
        return loadSimplePortfolio(data);
    } else { // E*Trade csv dump
        assert(header.at(0) == "Account Summary");
        assert(data.size() >= 16);
        return loadETradePortfolio(data);
    }
}

} // anonymous namespace

Portfolio::Portfolio(const CsvFile& csv, const Market& market)
    : m_holdings { loadHoldings(csv) }
    , m_market { market }
{
    // std::cout << "Portfolio::Portfolio" << std::endl;
}

Portfolio::Portfolio(HoldingsType holdings, const Market& market)
    : m_holdings { std::move(holdings) }
    , m_market { market }
{
    // std::cout << "Portfolio::Portfolio" << std::endl;
}

double Portfolio::totalValue(size_t i) const
{
    double total {};
    for (const auto& [symbol, quantity] : m_holdings) {
        const double value = m_market.get(symbol).ohlc().at(i).hl2() * quantity;
        total += value;
    }
    return total;
}

std::pair<double, std::set<std::string>> Portfolio::totalValue(AssetTag tag) const
{
    double total {};
    std::set<std::string> list;
    for (const auto& [symbol, quantity] : m_holdings) {
        const auto& asset = m_market.get(symbol);
        if (!asset.hasTag(tag)) {
            continue;
        }
        list.insert(symbol);
        const double value = asset.ohlc().at(0).hl2() * quantity;
        total += value;
    }
    return { total, list };
}

double Portfolio::valueChange(size_t i, size_t offset) const
{
    if (offset == 0) {
        assert(offset > 0);
        return 0; // No value change in same day
    }
    const auto today = totalValue(i);
    const auto yesterday = totalValue(i + offset);
    assert(yesterday > 0);
    return (today - yesterday) / yesterday;
}

double Portfolio::avgReturn() const
{
    // RP = w1R1 + w2R2
    const auto total = totalValue(0);
    if (total <= 0) {
        return 0;
    }
    double result {};
    for (const auto& [symbol, quantity] : m_holdings) {
        const auto& asset = m_market.get(symbol);
        const double value = asset.ohlc().at(0).hl2() * quantity;
        const double weight = value / total;
        result += weight * asset.avgReturn(0);
    }
    return result;
}

double Portfolio::avgRisk() const
{
    // σ²(portfolio) = ΣΣw(i)w(j)σ(i)σ(j)ρ(i,j)
    const auto total = totalValue(0);
    if (total <= 0) {
        return 0;
    }
    double result {};
    for (const auto& [symbol1, quantity1] : m_holdings) {
        const auto& asset1 = m_market.get(symbol1);
        const double value1 = asset1.ohlc().at(0).hl2() * quantity1;
        for (const auto& [symbol2, quantity2] : m_holdings) {
            const auto& asset2 = m_market.get(symbol2);
            const double value2 = asset2.ohlc().at(0).hl2() * quantity2;
            const double corr = symbol1 == symbol2 ? 1 : asset1.correlation(asset2, PriceType::HL2, false);
            result += (value1 / total) * (value2 / total) * corr * asset1.avgRisk(0) * asset2.avgRisk(0);
        }
    }
    return std::sqrt(result);
}

double Portfolio::avgReturn(int length) const
{
    assert(length > 0);
    double result {};
    for (int i = 0; i < length; ++i) {
        result += valueChange(i, 365);
    }
    return result / length;
}

double Portfolio::avgRisk(int length) const
{
    assert(length > 0);
    std::vector<double> vector;
    vector.reserve(length);
    for (int i = 0; i < length; ++i) {
        vector.push_back(valueChange(i, 365));
    }
    return Utils::stdDev(vector);
}

void Portfolio::saveAllocations(const std::string& filePath) const
{
    std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);
    if (!outFile.is_open()) {
        std::cout << "Portfolio::saveAllocations [FAILED TO OPEN FILE] " << filePath << std::endl;
    }

    // Header
    outFile << "tag,totalAmount,percent,symbols..." << std::endl;
    double total = totalValue(0);

    // Body
    for (AssetTag tag = AssetTag::ETF; tag <= AssetTag::Unclassified; tag = static_cast<AssetTag>(static_cast<int>(tag) + 1)) {
        const auto value = totalValue(tag);
        outFile << EnumUtils::to_string(tag) << ","
                << value.first << ","
                << std::round(100.0 * value.first / total) << ","
                << value.second.size();
        for (const auto& symbol : value.second) {
            outFile << "," << symbol;
        }
        outFile << std::endl;
    }
}

void Portfolio::saveSymbols(const std::string& filePath) const
{
    std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);
    if (!outFile.is_open()) {
        std::cout << "Portfolio::saveSymbols [FAILED TO OPEN FILE] " << filePath << std::endl;
    }

    outFile << "[";
    for (const auto& [symbol, quantity] : m_holdings) {
        outFile << "\"" << symbol << "\",";
    }
    outFile << "]" << std::endl
            << std::endl;

    for (const auto& [symbol, quantity] : m_holdings) {
        const auto& asset = m_market.get(symbol);
        outFile << symbol << "\t" << asset.yahoo("longName") << "\t" << asset.tags() << std::endl;
    }
}
