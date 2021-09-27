/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#pragma once

#include <QDateTime>

#include <string>
#include <vector>

namespace Farazlib {
namespace Utils {

    QDateTime toDateTime(const std::string& str);
    std::string join(const std::vector<std::string>& list, const std::string& delim);

    double mean(const std::vector<double>& vector);
    double stdDev(const std::vector<double>& vector);
    std::vector<double> rankify(const std::vector<double>& vector);

    double pearsonCorrelation(const std::vector<double>& x, const std::vector<double>& y);
    double spearmanCorrelation(const std::vector<double>& x, const std::vector<double>& y);

    std::pair<double, double> linearRegression(const std::vector<double>& x, const std::vector<double>& y);

    double doublingTime(double ratePercent);
    size_t powi(size_t base, size_t exp);

} // namespace Utils
} // namespace Farazlib
