/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "lib/Portfolio.hpp"
#include "lib/Asset.hpp"
#include "lib/Market.hpp"
#include "lib/Utils.hpp"

#include <gtest/gtest.h>

using namespace Farazlib;

constexpr double epsilon = 1e-3;

TEST(Portfolio, riskreturn1)
{
    AssetInfo info1;
    info1.avgRisk = 0.10;
    info1.avgReturn = 0.10;
    info1.correlation["B"] = 1;
    const Asset asset1 { "A", 1, info1 };

    AssetInfo info2;
    info2.avgRisk = 0.10;
    info2.avgReturn = 0.10;
    info2.correlation["A"] = 1;
    const Asset asset2 { "B", 1, info2 };

    const Market market { { asset1, asset2 } };

    Portfolio portfolio;
    portfolio.set(asset1.symbol(), 100);
    portfolio.set(asset2.symbol(), 100);

    EXPECT_NEAR(0.10, Utils::avgRisk(market, portfolio), epsilon);
    EXPECT_NEAR(0.10, Utils::avgReturn(market, portfolio), epsilon);
}

TEST(Portfolio, riskreturn2)
{
    AssetInfo info1;
    info1.avgRisk = 0.10;
    info1.avgReturn = 0.10;
    info1.correlation["B"] = 0;
    const Asset asset1 { "A", 1, info1 };

    AssetInfo info2;
    info2.avgRisk = 0.10;
    info2.avgReturn = 0.10;
    info2.correlation["A"] = 0;
    const Asset asset2 { "B", 1, info2 };

    const Market market { { asset1, asset2 } };

    Portfolio portfolio;
    portfolio.set(asset1.symbol(), 100);
    portfolio.set(asset2.symbol(), 100);

    EXPECT_NEAR(0.071, Utils::avgRisk(market, portfolio), epsilon);
    EXPECT_NEAR(0.10, Utils::avgReturn(market, portfolio), epsilon);
}
