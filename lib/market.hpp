/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#pragma once

#include "asset.hpp"

#include <map>

namespace Farazlib {

class Market {
public:
    /**
     * @brief Market Constructor
     * @param symbolsDir path to SYM.csv and SYM.json files
     * @param infoCsv loaded market.csv file
     * @param symbols list of symbols to load (default: all)
     */
    Market(const std::string& symbolsDir, const CsvFile& infoCsv, const std::set<std::string>& symbols = {});

    /**
     * @brief Market Constructor
     * @param assets list of assets
     */
    explicit Market(const std::vector<Asset>& assets);

    /**
     * @brief get Asset getter function
     * @param symbol Ticker symbol of an asset
     * @return const refrence to the asset
     */
    const Asset& get(const std::string& symbol) const;
    double correlation(const std::string& symbol1, const std::string& symbol2) const;

    void saveAssets(const std::string& symbolsDir) const; // Save ohlc data
    void saveCorrelationList(const std::string& filePath) const;
    void saveMarketInfo(const std::string& filePath) const;
    void saveSymbols(const std::string& filePath) const; // Save symbols array

    using AssetsDb = std::map<std::string, Asset>; ///< Symbol -> Asset Database

private:
    const AssetsDb m_assets; ///< Symbol to Asset hashmap
};

} // namespace Farazlib
