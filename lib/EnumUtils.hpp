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

namespace portopt::EnumUtils {

std::set<AssetClass> assetTag(const std::string& key);
std::string to_string(AssetClass tag);

} // namespace portopt::EnumUtils
