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
    const Asset asset1 { "VOO", 100, info };
    EXPECT_EQ("VOO", asset1.symbol());
    ASSERT_EQ(1, asset1.ohlc().size());
    EXPECT_EQ(100, asset1.ohlc().at(0).close);
}

TEST(Asset, Asset2)
{
    const AssetInfo info {};
    const Asset asset1 { "VOO", "../../data/test", info };
    EXPECT_EQ("VOO", asset1.symbol());
    ASSERT_EQ(2, asset1.ohlc().size());
    EXPECT_EQ(78, asset1.ohlc().at(0).close);
    EXPECT_EQ(77, asset1.ohlc().at(1).close);
}
