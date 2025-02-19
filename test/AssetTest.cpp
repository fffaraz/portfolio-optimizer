/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "lib/Asset.hpp"

#include <gtest/gtest.h>

using namespace Farazlib;

TEST(Asset, Asset1)
{
    const AssetInfo info {};
    const Asset asset1 { "AGG", 100, info };
    EXPECT_EQ("AGG", asset1.symbol());
    ASSERT_EQ(1, asset1.ohlc().size());
    EXPECT_EQ(100, asset1.ohlc().at(0).close);
}
