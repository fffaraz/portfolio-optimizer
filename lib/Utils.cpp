/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "Utils.hpp"

#include "EnumUtils.hpp"
#include "Market.hpp"
#include "Portfolio.hpp"

#include <cassert>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numbers>
#include <numeric>
#include <sstream>

using namespace Farazlib;
using namespace Farazlib::Utils;

std::string Utils::to_string(const TimePoint& tp)
{
    const auto tt = std::chrono::system_clock::to_time_t(tp);

    std::tm tmbuf {};
    std::tm* const tm = localtime_r(&tt, &tmbuf);
    assert(tm != nullptr);

#if 0
    std::array<char, 32> buf {};
    const size_t res = std::strftime(buf.data(), sizeof(buf), "%Y-%m-%d", tm);
    assert(res > 0);

    return { buf.data(), res };
#else
    std::ostringstream oss;
    oss << std::put_time(tm, "%F"); // %F is equivalent to "%Y-%m-%d"
    return oss.str();
#endif
}

TimePoint Utils::toTimePoint(const std::string& str)
{
    assert(str.size() >= 10); // "YYYY-MM-DD"
    const std::string dateOnly = str.substr(0, 10);
#if 0
    const std::chrono::year y { std::stoi(dateOnly.substr(0, 4)) };
    const std::chrono::month m { static_cast<unsigned int>(std::stoi(dateOnly.substr(5, 2))) };
    const std::chrono::day d { static_cast<unsigned int>(std::stoi(dateOnly.substr(8, 2))) };
    const std::chrono::year_month_day ymd { y, m, d };
    return std::chrono::system_clock::from_time_t(std::chrono::system_clock::to_time_t(std::chrono::sys_days(ymd)));
#else
    std::tm tm {};
    std::istringstream iss(dateOnly);
    iss >> std::get_time(&tm, "%Y-%m-%d");
    // assert(iss.good());
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
#endif
}

std::string Utils::join(const std::vector<std::string>& list, const std::string& delim)
{
    std::string result;
    for (const auto& str : list) {
        if (!result.empty()) {
            result += delim;
        }
        result += str;
    }
    return result;
}

double Utils::mean(const std::vector<double>& vector)
{
    if (vector.empty()) {
        return 0;
    }
    const double sum = std::accumulate(vector.begin(), vector.end(), 0.0);
    return sum / static_cast<double>(vector.size());
    // std::reduce(std::execution::par_unseq, std::begin(vector), std::end(vector));
}

double Utils::stdDev(const std::vector<double>& vector)
{
    if (vector.empty()) {
        return 0;
    }
    const double m = mean(vector);
    double sum_deviation {};
    for (const double d : vector) {
        const double deviation = d - m;
        sum_deviation += deviation * deviation;
    }
    return std::sqrt(sum_deviation / static_cast<double>(vector.size()));
}

std::vector<double> Utils::rankify(const std::vector<double>& vector)
{
    std::vector<double> result;
    result.reserve(vector.size());
    for (std::size_t i = 0; i < vector.size(); ++i) {
        int r { 1 };
        int s { 1 };
        // Count no of smaller elements in 0 to i-1 and i+1 to N-1
        for (std::size_t j = 0; j < vector.size(); j++) {
            if (i == j) {
                continue;
            }
            if (vector.at(j) < vector.at(i)) {
                r++;
            }
            if (vector.at(j) == vector.at(i)) {
                s++;
            }
        }

        // Use Fractional Rank formula
        result.push_back(r + ((s - 1) * 0.5));
    }
    return result;
}

double Utils::pearsonCorrelation(const std::vector<double>& x, const std::vector<double>& y)
{
    // https://en.wikipedia.org/wiki/Pearson_correlation_coefficient
    assert(!x.empty());
    assert(x.size() == y.size());
    if (x.empty() || x.size() != y.size()) {
        return 0;
    }

    const double mean1 = Utils::mean(x);
    const double mean2 = Utils::mean(y);

    double cov {}; // covariance
    double std1 {}; // standard deviation x
    double std2 {}; // standard deviation y
    for (std::size_t i = 0; i < x.size(); ++i) {
        const double diff1 = x.at(i) - mean1;
        const double diff2 = y.at(i) - mean2;
        cov += diff1 * diff2;
        std1 += diff1 * diff1;
        std2 += diff2 * diff2;
    }
    cov /= static_cast<double>(x.size());
    std1 = std::sqrt(std1 / static_cast<double>(x.size()));
    std2 = std::sqrt(std2 / static_cast<double>(x.size()));

    assert(std1 > 0);
    assert(std2 > 0);
    if (std1 <= 0 || std2 <= 0) {
        return 0;
    }
    return cov / (std1 * std2);
}

double Utils::spearmanCorrelation(const std::vector<double>& x, const std::vector<double>& y)
{
    // https://en.wikipedia.org/wiki/Spearman%27s_rank_correlation_coefficient
    const auto rank1 = Utils::rankify(x);
    const auto rank2 = Utils::rankify(y);
    return pearsonCorrelation(rank1, rank2);
}

std::pair<double, double> Utils::linearRegression(const std::vector<double>& x, const std::vector<double>& y)
{
    assert(!x.empty());
    assert(x.size() == y.size());
    if (x.empty() || x.size() != y.size()) {
        return {};
    }
    const auto n = static_cast<double>(x.size());
    const auto s_x = std::accumulate(x.begin(), x.end(), 0.0);
    const auto s_y = std::accumulate(y.begin(), y.end(), 0.0);
    const auto s_xx = std::inner_product(x.begin(), x.end(), x.begin(), 0.0);
    const auto s_xy = std::inner_product(x.begin(), x.end(), y.begin(), 0.0);
    const auto slop = (n * s_xy - s_x * s_y) / (n * s_xx - s_x * s_x);
    const auto intercept = (s_y - slop * s_x) / n;
    return { slop, intercept };
}

double Utils::doublingTime(double ratePercent)
{
    return std::numbers::ln2 / std::log(1 + (ratePercent / 100));
}

std::size_t Utils::powi(std::size_t base, std::size_t exp)
{
    std::size_t result = 1;
    while (exp > 0) {
        if ((exp & 1U) == 1) {
            result *= base;
        }
        exp /= 2;
        base *= base;
    }
    return result;
}

double Utils::avgRisk(const Market& market, const Portfolio& portfolio)
{
    // σ²(portfolio) = ΣΣw(i)w(j)σ(i)σ(j)ρ(i,j)
    const auto total = totalValue(market, portfolio, 0);
    if (total <= 0) {
        return 0;
    }
    double result {};
    for (const auto& [symbol1, quantity1] : portfolio.holdings()) {
        const auto& asset1 = market.get(symbol1);
        const double value1 = asset1.ohlc().at(0).hl2() * quantity1;
        for (const auto& [symbol2, quantity2] : portfolio.holdings()) {
            const auto& asset2 = market.get(symbol2);
            const double value2 = asset2.ohlc().at(0).hl2() * quantity2;
            const double corr = symbol1 == symbol2 ? 1 : asset1.correlation(asset2, PriceType::HL2, false);
            result += (value1 / total) * (value2 / total) * corr * asset1.avgRisk(0) * asset2.avgRisk(0);
        }
    }
    return std::sqrt(result);
}

double Utils::avgRisk(const Market& market, const Portfolio& portfolio, int length)
{
    assert(length > 0);
    std::vector<double> vector;
    vector.reserve(length);
    for (int i = 0; i < length; ++i) {
        vector.push_back(valueChange(market, portfolio, i, 365));
    }
    return Utils::stdDev(vector);
}

double Utils::avgReturn(const Market& market, const Portfolio& portfolio)
{
    // RP = w1R1 + w2R2
    const auto total = totalValue(market, portfolio, 0);
    if (total <= 0) {
        return 0;
    }
    double result {};
    for (const auto& [symbol, quantity] : portfolio.holdings()) {
        const auto& asset = market.get(symbol);
        const double value = asset.ohlc().at(0).hl2() * quantity;
        const double weight = value / total;
        result += weight * asset.avgReturn(0);
    }
    return result;
}

double Utils::avgReturn(const Market& market, const Portfolio& portfolio, int length)
{
    assert(length > 0);
    double result {};
    for (int i = 0; i < length; ++i) {
        result += valueChange(market, portfolio, i, 365);
    }
    return result / length;
}

void Utils::saveAllocations(const Market& market, const Portfolio& portfolio, const std::string& filePath)
{
    std::cerr << "\nPortfolio::saveAllocations\n";
    std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);
    if (!outFile.is_open()) {
        std::cerr << "Portfolio::saveAllocations [FAILED TO OPEN FILE] " << filePath << "\n";
    }

    // Header
    outFile << "tag,totalAmount,percent,symbols...\n";
    const double total = totalValue(market, portfolio, 0);

    // Body
    for (AssetTag tag = AssetTag::Unclassified; tag < AssetTag::LastTag; tag = static_cast<AssetTag>(static_cast<int>(tag) + 1)) {
        const auto value = totalValue(market, portfolio, tag);
        outFile << EnumUtils::to_string(tag) << ","
                << value.first << ","
                << std::round(100.0 * value.first / total) << ","
                << value.second.size();
        for (const auto& symbol : value.second) {
            outFile << "," << symbol;
        }
        outFile << "\n";
    }
}

double Utils::totalValue(const Market& market, const Portfolio& portfolio, std::size_t i)
{
    double total {};
    for (const auto& [symbol, quantity] : portfolio.holdings()) {
        const double value = market.get(symbol).ohlc().at(i).hl2() * quantity;
        total += value;
    }
    return total;
}

std::pair<double, std::set<std::string>> Utils::totalValue(const Market& market, const Portfolio& portfolio, AssetTag tag)
{
    double total {};
    std::set<std::string> list;
    for (const auto& [symbol, quantity] : portfolio.holdings()) {
        const auto& asset = market.get(symbol);
        if (!asset.hasTag(tag)) {
            continue;
        }
        list.insert(symbol);
        const double value = asset.ohlc().at(0).hl2() * quantity;
        total += value;
    }
    return { total, list };
}

double Utils::valueChange(const Market& market, const Portfolio& portfolio, std::size_t i, std::size_t offset)
{
    if (offset == 0) {
        assert(offset > 0);
        return 0; // No value change in same day
    }
    const auto today = totalValue(market, portfolio, i);
    const auto yesterday = totalValue(market, portfolio, i + offset);
    assert(yesterday > 0);
    return (today - yesterday) / yesterday;
}
