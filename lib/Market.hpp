/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#pragma once

#include "Asset.hpp"

#include <map>

namespace portopt {

class Market {
public:
    /**
     * @brief Market Constructor
     * @param symbolsDir path to SYM.csv and SYM.json files
     * @param infoCsv loaded market.csv file
     * @param symbols list of symbols to load (default: all)
     */
    Market(const std::filesystem::path& symbolsDir, const CsvFile& infoCsv, const std::set<std::string>& symbols = {});

    /**
     * @brief Market Constructor
     * @param assets list of loaded assets
     */
    explicit Market(const std::vector<Asset>& assets);

    /**
     * @brief get Asset getter function
     * @param symbol Ticker symbol of an asset
     * @return const refrence to the asset
     */
    [[nodiscard]] const Asset& get(const std::string& symbol) const;

    /**
     * @brief correlation
     * @param symbol1
     * @param symbol2
     * @return
     */
    [[nodiscard]] double correlation(const std::string& symbol1, const std::string& symbol2) const;

    void saveAssets(const std::filesystem::path& symbolsDir) const; // Save ohlc data
    void saveCorrelationList(const std::filesystem::path& filePath) const;
    void saveMarketInfo(const std::filesystem::path& filePath) const;
    void saveSymbols(const std::filesystem::path& filePath) const; // Save symbols array

private:
    const std::map<std::string, Asset> m_assets; ///< Symbol to Asset hashmap
};

} // namespace portopt
