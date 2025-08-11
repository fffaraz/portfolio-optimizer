/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "AssetRatio.hpp"

using namespace portopt;

AssetRatio::AssetRatio(const Asset& asset1, const Asset& asset2)
    : m_asset1 { asset1 }
    , m_asset2 { asset2 }
{
}
