/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#pragma once

#include "csvfile.hpp"

#include <string>
#include <unordered_map>

namespace Farazlib {

struct AssetInfo {
    AssetInfo() = default;
    explicit AssetInfo(const CsvFile::RecordType& info);

    double dividendYield {};
    double expenseRatio {};

    double avgRisk {};
    double avgReturn {};
    std::unordered_map<std::string, double> correlation; // Symbol -> Correlation Coefficient

    // uint64_t marketCap {};
    // std::string fundCategory {};
    // std::string category{};
    // std::string country{};
    // int fullTimeEmployees{};
    // std::string fundFamily{}; // ETF only
    // uint64_t fundInceptionDate{}; // Unix Timestamp
    // std::string industry{};
    // std::string legalType{};
    // std::string longBusinessSummary{};
    // std::string longName{};
    // std::string quoteType{}; // EQUITY, ETF
    // std::string sector{};
    // uint64_t sharesShort{};
    // uint64_t sharesOutstanding{};
    // std::string shortName{};
    // uint64_t totalAssets{}; // ETF only, Net Assets
};

} // namespace Farazlib
