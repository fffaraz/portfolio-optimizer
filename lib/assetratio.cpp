/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "assetratio.hpp"

using namespace Farazlib;

AssetRatio::AssetRatio(const Asset& asset1, const Asset& asset2)
    : m_asset1 { asset1 }
    , m_asset2 { asset2 }
{
}
