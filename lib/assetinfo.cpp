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
    if (info.size() != 3) {
        std::cout << "AssetInfo::AssetInfo [info.size() != 3] info.size: " << info.size() << std::endl;
        // assert(info.size() == 3);
        return;
    }
    try {
        dividendYield = std::stod(info.at(1));
    } catch (...) {
    }
    try {
        expenseRatio = std::stod(info.at(2));
    } catch (...) {
    }
}
