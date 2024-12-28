/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#pragma once

#include "AssetEnums.hpp"
#include "AssetInfo.hpp"
#include "OhlcList.hpp"

#include <nlohmann/json.hpp>

#include <optional>
#include <set>
#include <string>

namespace Farazlib {

class Asset {
public:
    /**
     * @brief Construct a simple asset from symbol and price
     * @param symbol Ticker symbol
     * @param price price of the asset per share in USD
     * @param info extra asset attributes
     */
    Asset(std::string symbol, double price, AssetInfo info);

    /**
     * @brief Asset Constructor
     * @param symbol Ticker symbol
     * @param dataDir directory path for SYM.csv and SYM.json files
     * @param info extra asset attributes
     */
    Asset(std::string symbol, const std::string& dataDir, AssetInfo info);

    const std::string& symbol() const noexcept { return m_symbol; }
    const OhlcList& ohlc() const noexcept { return m_ohlc; }
    const AssetInfo& info() const noexcept { return m_info; }
    bool hasTag(AssetTag tag) const { return m_tags.contains(tag); }

    std::string yahoo(const std::string& key) const; ///< Returns value from yahoo json
    void save(const std::string& dataDir) const; ///< save ohlc to a symbol.csv file

    double correlation(const Asset& other, PriceType priceType, bool rankify) const;
    double avgRisk(size_t length) const;
    double avgReturn(size_t length) const;

    std::string tags() const; ///< returns asset's tags concatenated
    bool isETF() const; ///< returns true if the asset is an ETF
    bool isBond() const; ///< returns true if the asset is a bond
    bool isForeign() const; ///< returns true if the asset is a Foreign asset
    bool isREIT() const; ///< returns true if the asset is a REIT
    std::optional<AssetTag> management() const; ///< returns ETF asset management company

private:
    const std::string m_symbol; ///< Ticker symbol
    const OhlcList m_ohlc; ///< Open-high-low-close chart
    const nlohmann::json m_yahoo; ///< Yahoo Finance Json
    const AssetInfo m_info; ///< Other asset attributes
    const std::set<AssetTag> m_tags; ///< Tags
};

} // namespace Farazlib
