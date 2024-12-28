/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#pragma once

#include "AssetEnums.hpp"

#include <set>
#include <string>

namespace Farazlib {
namespace EnumUtils {

    std::set<AssetTag> assetTag(const std::string& key);
    std::string to_string(AssetTag tag);

} // namespace EnumUtils
} // namespace Farazlib
