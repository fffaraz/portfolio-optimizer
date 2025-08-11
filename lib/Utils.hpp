/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#pragma once

#include "AssetEnums.hpp"
#include "TimePoint.hpp"

#include <set>
#include <string>
#include <utility>
#include <vector>

namespace portopt {

class Market;
class Portfolio;

namespace Utils {

    std::string to_string(const TimePoint& tp);
    TimePoint toTimePoint(const std::string& str);
    std::string join(const std::vector<std::string>& list, const std::string& delim);

    double avgRisk(const Market& market, const Portfolio& portfolio);
    double avgReturn(const Market& market, const Portfolio& portfolio);
    double avgRisk(const Market& market, const Portfolio& portfolio, int length);
    double avgReturn(const Market& market, const Portfolio& portfolio, int length);

    double totalValue(const Market& market, const Portfolio& portfolio, std::size_t i = 0);
    std::pair<double, std::set<std::string>> totalValue(const Market& market, const Portfolio& portfolio, AssetTag tag);
    double valueChange(const Market& market, const Portfolio& portfolio, std::size_t i, std::size_t offset);

    void saveAllocations(const Market& market, const Portfolio& portfolio, const std::string& filePath);

    double mean(const std::vector<double>& vector);
    double stdDev(const std::vector<double>& vector);
    std::vector<double> rankify(const std::vector<double>& vector);

    double pearsonCorrelation(const std::vector<double>& x, const std::vector<double>& y);
    double spearmanCorrelation(const std::vector<double>& x, const std::vector<double>& y);

    std::pair<double, double> linearRegression(const std::vector<double>& x, const std::vector<double>& y);

    double doublingTime(double ratePercent);
    std::size_t powi(std::size_t base, std::size_t exp);

} // namespace Utils
} // namespace portopt
