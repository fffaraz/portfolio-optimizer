/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "lib/OhlcList.hpp"

#include <gtest/gtest.h>

using namespace Farazlib;

TEST(OhlcList, sameDay)
{
    const Ohlc ohlc1 { 95, 100, 90, 92 };
    const Ohlc ohlc0 { 95, 100, 90, 98 };
    const OhlcList list { { ohlc0, ohlc1 } };

    EXPECT_EQ(PriceDirection::Up, list.priceDirection(0, 0)); // close > open => Up
    EXPECT_EQ(10.0 / 90, list.priceChange(0, 0, PriceType::Close));

    EXPECT_EQ(PriceDirection::Down, list.priceDirection(1, 0)); // close < open => Down
    EXPECT_EQ(10.0 / 90, list.priceChange(1, 0, PriceType::Close));
}

TEST(OhlcList, priceDirection1)
{
    /*
        -
        |
        |
        -
            -
            |
            |
            -
    */
    const Ohlc ohlc1 { 95, 100, 90, 95 };
    const Ohlc ohlc0 { 75, 80, 70, 75 };
    const OhlcList list { { ohlc0, ohlc1 } };
    EXPECT_EQ(PriceDirection::VeryDown, list.priceDirection(0, 1));
}

TEST(OhlcList, priceDirection2)
{
    /*
        -
        |
        |
        -   -
            |
            |
            -
    */
    const Ohlc ohlc1 { 95, 100, 90, 95 };
    const Ohlc ohlc0 { 85, 90, 80, 85 };
    const OhlcList list { { ohlc0, ohlc1 } };
    EXPECT_EQ(PriceDirection::VeryDown, list.priceDirection(0, 1));
}

TEST(OhlcList, priceDirection3)
{
    /*
        -
        |
        |   -
        -   |
            |
            -
    */
    const Ohlc ohlc1 { 100, 110, 90, 105 };
    const Ohlc ohlc0 { 85, 95, 80, 85 };
    const OhlcList list { { ohlc0, ohlc1 } };
    EXPECT_EQ(PriceDirection::Down, list.priceDirection(0, 1));
}

TEST(OhlcList, priceDirection4)
{
    /*
        -   -
        |   |
        |   |
        -   -
    */
    const Ohlc ohlc1 { 100, 110, 90, 105 };
    const Ohlc ohlc0 { 100, 110, 90, 105 };
    const OhlcList list { { ohlc0, ohlc1 } };
    EXPECT_EQ(PriceDirection::Narrow, list.priceDirection(0, 1));
}

TEST(OhlcList, priceDirection5)
{
    /*
            -
        -   |
        |   |
        |   |
        -   |
            -
    */
    const Ohlc ohlc1 { 100, 110, 90, 105 };
    const Ohlc ohlc0 { 100, 120, 80, 105 };
    const OhlcList list { { ohlc0, ohlc1 } };
    EXPECT_EQ(PriceDirection::Widen, list.priceDirection(0, 1));
}

TEST(OhlcList, priceDirection6)
{
    /*
            -
        -   |
        |   |
        |   |
        -   -
    */
    const Ohlc ohlc1 { 100, 110, 90, 105 };
    const Ohlc ohlc0 { 100, 120, 90, 105 };
    const OhlcList list { { ohlc0, ohlc1 } };
    EXPECT_EQ(PriceDirection::Up, list.priceDirection(0, 1));
}

TEST(OhlcList, priceDirection7)
{
    /*
            -
        -   |
        |   |
        |   -
        -
    */
    const Ohlc ohlc1 { 100, 110, 90, 105 };
    const Ohlc ohlc0 { 100, 120, 95, 105 };
    const OhlcList list { { ohlc0, ohlc1 } };
    EXPECT_EQ(PriceDirection::Up, list.priceDirection(0, 1));
}

TEST(OhlcList, priceDirection8)
{
    /*
            -
            |
        -   -
        |
        |
        -
    */
    const Ohlc ohlc1 { 100, 110, 90, 105 };
    const Ohlc ohlc0 { 115, 120, 110, 115 };
    const OhlcList list { { ohlc0, ohlc1 } };
    EXPECT_EQ(PriceDirection::VeryUp, list.priceDirection(0, 1));
}
