/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "assetinfo.hpp"

#include <iostream>

using namespace Farazlib;

AssetInfo::AssetInfo(const CsvFile::RecordType& info)
{
    if (info.size() != 6) {
        std::cout << "AssetInfo::AssetInfo [info.size() != 6] " << info.size() << std::endl;
        // assert(info.size() == 6);
        return;
    }
    description = info.at(1);
    try {
        marketCap = std::stoll(info.at(2));
    } catch (...) {
    }
    try {
        dividendYield = std::stod(info.at(3));
    } catch (...) {
    }
    try {
        expenseRatio = std::stod(info.at(4));
    } catch (...) {
    }
    fundCategory = info.at(5);
}
