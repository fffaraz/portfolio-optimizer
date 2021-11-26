/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "asset.hpp"
#include "enumutils.hpp"
#include "utils.hpp"

#include <QJsonDocument>
#include <QJsonObject>

#include <fstream>
#include <iostream>
#include <streambuf>

using namespace Farazlib;

namespace {

QJsonObject loadJsonFile(const std::string& filePath)
{
    std::ifstream ifs(filePath);
    std::string buffer((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    QJsonDocument document = QJsonDocument::fromJson(QString::fromStdString(buffer).toUtf8());
    return document.object();
}

} // anonymous namespace

Asset::Asset(std::string symbol, double price, AssetInfo info)
    : m_symbol { std::move(symbol) }
    , m_ohlc { price }
    , m_info { std::move(info) }
    , m_tags { AssetTag::Unclassified }
{
    std::cout << "Asset::Asset " << m_symbol << " ohlc.size:" << m_ohlc.size() << std::endl;
}

Asset::Asset(std::string symbol, const std::string& dataDir, AssetInfo info)
    : m_symbol { std::move(symbol) }
    , m_ohlc { OhlcList { CsvFile { dataDir + "/" + m_symbol + ".csv", true }, OhlcTimeFrame::Daily } }
    , m_yahoo { loadJsonFile(dataDir + "/" + m_symbol + ".json") }
    , m_info { std::move(info) }
    , m_tags { findTags() } // Must be last
{
    std::cout << "Asset::Asset " << m_symbol << " ohlc.size:" << m_ohlc.size() << std::endl;
}

std::set<AssetTag> Asset::findTags() const
{
    std::set<AssetTag> result;

    if (isETF()) {
        result.insert(AssetTag::ETF);
        // Only ETFs have tags based on their symbol
        auto symbolTags = EnumUtils::assetTag(m_symbol);
        result.insert(symbolTags.begin(), symbolTags.end());
    } else {
        result.insert(AssetTag::NotETF);
    }
    if (isBond()) {
        result.insert(AssetTag::Bond);
    }
    if (isForeign()) {
        result.insert(AssetTag::Foreign);
    }
    if (isREIT()) {
        result.insert(AssetTag::REIT);
    }

    auto categoryTags = EnumUtils::assetTag(yahoo("category"));
    result.insert(categoryTags.begin(), categoryTags.end());

    auto sectorTags = EnumUtils::assetTag(yahoo("sector"));
    result.insert(sectorTags.begin(), sectorTags.end());

    if (result.size() < 1) {
        result.insert(AssetTag::Unclassified);
    }

    const auto mngt = management();
    if (mngt.has_value()) {
        result.insert(mngt.value());
    }

    return result;
}

std::string Asset::tags() const
{
    std::vector<std::string> result;
    result.reserve(m_tags.size());
    for (AssetTag tag : m_tags) {
        result.push_back(EnumUtils::to_string(tag));
    }
    return Utils::join(result, ", ");
}

std::string Asset::yahoo(const std::string& key) const
{
    const QString qKey = QString::fromStdString(key);
    if (!m_yahoo.contains(qKey)) {
        std::cout << "Asset::yahoo [not found] " << m_symbol << "\t" << key << std::endl;
    }
    QString result = m_yahoo.value(qKey).toString();
    return result.replace(",", "").toStdString();
}

void Asset::save(const std::string& dataDir) const
{
    m_ohlc.save(dataDir + "/" + m_symbol + ".csv");
}

double Asset::correlation(const Asset& other, const PriceType priceType, const bool rankify) const
{
    if (m_symbol == other.m_symbol) {
        return 1;
    }
    if (m_ohlc.size() < 2) {
        return m_info.correlation.contains(other.m_symbol) ? m_info.correlation.at(other.m_symbol) : 0;
    }
    if (other.m_ohlc.size() < 2) {
        return other.m_info.correlation.contains(m_symbol) ? other.m_info.correlation.at(m_symbol) : 0;
    }
    const size_t maxSize = 400; // TODO: parameterize
    const size_t size = std::min({ m_ohlc.size(), other.m_ohlc.size(), maxSize });
    if (!m_ohlc.matchDatetime(other.m_ohlc, size)) {
        std::cout << "Asset::correlation [datetime mismatch] " << m_symbol << " " << other.m_symbol << std::endl;
        return 0;
    }
    const auto vector1 = m_ohlc.vector(size, 0, priceType);
    const auto vector2 = other.m_ohlc.vector(size, 0, priceType);
    return rankify ? Utils::spearmanCorrelation(vector1, vector2) : Utils::pearsonCorrelation(vector1, vector2);
}

double Asset::avgRisk(size_t length) const
{
    return m_ohlc.size() > 1 ? m_ohlc.avgRisk(length) : m_info.avgRisk;
}

double Asset::avgReturn(size_t length) const
{
    return m_ohlc.size() > 1 ? m_ohlc.avgReturn(length) : m_info.avgReturn;
}

bool Asset::isETF() const
{
    return yahoo("quoteType") == "ETF";
}

bool Asset::isBond() const
{
    if (m_symbol == "VMBS") { // Vanguard Mortgage-Backed Securities ETF
        return true;
    }
    if (yahoo("category").find("Bond") != std::string::npos) {
        return true;
    }
    if (yahoo("longName").find(" Bond ") != std::string::npos) {
        return true;
    }
    if (yahoo("longName").find(" Treasury Index Fund ") != std::string::npos) { // $EDV
        return true;
    }
    return false;
}

bool Asset::isForeign() const
{
    const auto category = yahoo("category");
    if (category.find("China") != std::string::npos) {
        return true;
    }
    if (category.find("Emerging") != std::string::npos) {
        return true;
    }
    if (category.find("Europe") != std::string::npos) {
        return true;
    }
    if (category.find("Pacific/Asia") != std::string::npos) {
        return true;
    }
    if (category.find("Foreign") != std::string::npos) {
        return true;
    }

    const auto name = yahoo("longName");
    if (name.find("Emerging Markets") != std::string::npos) {
        return true;
    }
    if (name.find(" ex-U") != std::string::npos) { // ex-U.S. OR ex-US
        return true;
    }

    const auto country = yahoo("country");
    if (!country.empty() && country != "United States") {
        return true;
    }
    return false;
}

bool Asset::isREIT() const
{
    if (yahoo("category").find("Real Estate") != std::string::npos) {
        return true;
    }
    return false;
}

std::optional<AssetTag> Asset::management() const
{
    const auto name = yahoo("longName");
    if (name.find("iShares") != std::string::npos) {
        return AssetTag::BlackRock;
    }
    if (name.find("Vanguard") != std::string::npos) {
        return AssetTag::Vanguard;
    }
    if (name.find("Schwab") != std::string::npos) {
        return AssetTag::Schwab;
    }
    if (name.find("SPDR") != std::string::npos) {
        return AssetTag::SPDR;
    }
    if (name.find("Invesco") != std::string::npos) {
        return AssetTag::Invesco;
    }
    return {};
}
