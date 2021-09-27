/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "utils.hpp"

#include <cmath>
#include <iostream>

using namespace Farazlib;

QDateTime Utils::toDateTime(const std::string& str)
{
    assert(str.size() == 10);
    return QDateTime::fromString(QString::fromStdString(str), "yyyy-MM-dd");
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
    return sum / vector.size();
    // std::reduce(std::execution::par_unseq, std::begin(vector), std::end(vector));
}

double Utils::stdDev(const std::vector<double>& vector)
{
    if (vector.empty()) {
        return 0;
    }
    const double m = mean(vector);
    double sum_deviation {};
    for (double d : vector) {
        double deviation = d - m;
        sum_deviation += deviation * deviation;
    }
    return std::sqrt(sum_deviation / vector.size());
}

std::vector<double> Utils::rankify(const std::vector<double>& vector)
{
    std::vector<double> result;
    result.reserve(vector.size());
    for (size_t i = 0; i < vector.size(); ++i) {
        int r { 1 };
        int s { 1 };
        // Count no of smaller elements in 0 to i-1 and i+1 to N-1
        for (size_t j = 0; j < vector.size(); j++) {
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
        result.push_back(r + (s - 1) * 0.5);
    }
    return result;
}

double Utils::pearsonCorrelation(const std::vector<double>& x, const std::vector<double>& y)
{
    // https://en.wikipedia.org/wiki/Pearson_correlation_coefficient
    assert(x.size() > 0);
    assert(x.size() == y.size());
    if (x.size() == 0 || x.size() != y.size()) {
        return 0;
    }

    const double mean1 = Utils::mean(x);
    const double mean2 = Utils::mean(y);

    double cov {}; // covariance
    double std1 {}; // standard deviation x
    double std2 {}; // standard deviation y
    for (size_t i = 0; i < x.size(); ++i) {
        const double diff1 = x.at(i) - mean1;
        const double diff2 = y.at(i) - mean2;
        cov += diff1 * diff2;
        std1 += diff1 * diff1;
        std2 += diff2 * diff2;
    }
    cov /= x.size();
    std1 = std::sqrt(std1 / x.size());
    std2 = std::sqrt(std2 / x.size());

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
    assert(x.size() > 0);
    assert(x.size() == y.size());
    if (x.size() == 0 || x.size() != y.size()) {
        return {};
    }
    const auto n = x.size();
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
    return std::log(2) / std::log(1 + ratePercent / 100);
}

size_t Utils::powi(size_t base, size_t exp)
{
    int result = 1;
    while (exp) {
        if (exp & 1) {
            result *= base;
        }
        exp >>= 1;
        base *= base;
    }
    return result;
}
