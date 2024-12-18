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
    Ohlc ohlc1 { 95, 100, 90, 92 };
    Ohlc ohlc0 { 95, 100, 90, 98 };
    OhlcList list { { ohlc0, ohlc1 } };
    EXPECT_EQ(PriceDirection::Up, list.priceDirection(0, 0));
    EXPECT_EQ(3.0 / 95, list.priceChange(0, 0, PriceType::Close));
    EXPECT_EQ(PriceDirection::Down, list.priceDirection(1, 0));
    EXPECT_EQ(-3.0 / 95, list.priceChange(1, 0, PriceType::Close));
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
    Ohlc ohlc1 { 95, 100, 90, 95 };
    Ohlc ohlc0 { 75, 80, 70, 75 };
    OhlcList list { { ohlc0, ohlc1 } };
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
    Ohlc ohlc1 { 95, 100, 90, 95 };
    Ohlc ohlc0 { 85, 90, 80, 85 };
    OhlcList list { { ohlc0, ohlc1 } };
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
    Ohlc ohlc1 { 100, 110, 90, 105 };
    Ohlc ohlc0 { 85, 95, 80, 85 };
    OhlcList list { { ohlc0, ohlc1 } };
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
    Ohlc ohlc1 { 100, 110, 90, 105 };
    Ohlc ohlc0 { 100, 110, 90, 105 };
    OhlcList list { { ohlc0, ohlc1 } };
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
    Ohlc ohlc1 { 100, 110, 90, 105 };
    Ohlc ohlc0 { 100, 120, 80, 105 };
    OhlcList list { { ohlc0, ohlc1 } };
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
    Ohlc ohlc1 { 100, 110, 90, 105 };
    Ohlc ohlc0 { 100, 120, 90, 105 };
    OhlcList list { { ohlc0, ohlc1 } };
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
    Ohlc ohlc1 { 100, 110, 90, 105 };
    Ohlc ohlc0 { 100, 120, 95, 105 };
    OhlcList list { { ohlc0, ohlc1 } };
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
    Ohlc ohlc1 { 100, 110, 90, 105 };
    Ohlc ohlc0 { 115, 120, 110, 115 };
    OhlcList list { { ohlc0, ohlc1 } };
    EXPECT_EQ(PriceDirection::VeryUp, list.priceDirection(0, 1));
}
