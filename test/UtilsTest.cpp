/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#include "lib/Utils.hpp"

#include <gtest/gtest.h>

using namespace portopt;

constexpr double epsilon = 1e-3;

TEST(Utils, toTimePoint)
{
    EXPECT_EQ(Utils::to_string(Utils::toTimePoint("2000-01-01")), "2000-01-01");
    EXPECT_EQ(Utils::to_string(Utils::toTimePoint("2000-01-02")), "2000-01-02");
    EXPECT_EQ(Utils::to_string(Utils::toTimePoint("2000-02-01")), "2000-02-01");
    EXPECT_EQ(Utils::to_string(Utils::toTimePoint("2000-02-02")), "2000-02-02");
    EXPECT_EQ(Utils::to_string(Utils::toTimePoint("2020-11-28")), "2020-11-28");
}

TEST(Utils, join)
{
    EXPECT_EQ("a, b, c", Utils::join({ "a", "b", "c" }, ", "));
}

TEST(Utils, mean)
{
    EXPECT_EQ(3, Utils::mean({ 1, 2, 3, 4, 5 }));
    EXPECT_EQ(0, Utils::mean({ -2, -1, 1, 2 }));
}

TEST(Utils, stdDev)
{
    EXPECT_EQ(0, Utils::stdDev({ 2, 2, 2, 2, 2 }));
}

TEST(Utils, rankify)
{
    const auto result = std::vector<double> { 4, 2.5, 2.5, 1 };
    EXPECT_EQ(result, Utils::rankify({ 3, 2, 2, 1 }));
}

TEST(Utils, pearsonCorrelation)
{
    EXPECT_NEAR(1, Utils::pearsonCorrelation({ 1, 2, 3, 4 }, { 5, 6, 7, 8 }), epsilon);
    EXPECT_NEAR(0, Utils::pearsonCorrelation({ 1, 2, 2, 1 }, { 5, 6, 5, 6 }), epsilon);
    EXPECT_NEAR(-1, Utils::pearsonCorrelation({ 1, 2, 3, 4 }, { 8, 7, 6, 5 }), epsilon);
}

TEST(Utils, spearmanCorrelation)
{
    EXPECT_NEAR(1, Utils::spearmanCorrelation({ 1, 2, 3, 4 }, { 5, 6, 7, 8 }), epsilon);
    EXPECT_NEAR(0, Utils::spearmanCorrelation({ 1, 2, 2, 1 }, { 5, 6, 5, 6 }), epsilon);
    EXPECT_NEAR(-1, Utils::spearmanCorrelation({ 1, 2, 3, 4 }, { 8, 7, 6, 5 }), epsilon);
}

TEST(Utils, linearRegression)
{
    const auto result = std::pair<double, double>(2, 3);
    EXPECT_EQ(result, Utils::linearRegression({ 1, 2, 3, 4 }, { 5, 7, 9, 11 }));
}

TEST(Utils, doublingTime)
{
    // https://en.wikipedia.org/wiki/Rule_of_72
    // https://en.wikipedia.org/wiki/Doubling_time
    EXPECT_NEAR(277.605, Utils::doublingTime(0.25), epsilon);
    EXPECT_NEAR(138.976, Utils::doublingTime(0.5), epsilon);
    EXPECT_NEAR(69.661, Utils::doublingTime(1), epsilon);
    EXPECT_NEAR(35.003, Utils::doublingTime(2), epsilon);
    EXPECT_NEAR(23.450, Utils::doublingTime(3), epsilon);
    EXPECT_NEAR(17.673, Utils::doublingTime(4), epsilon);
    EXPECT_NEAR(14.207, Utils::doublingTime(5), epsilon);
    EXPECT_NEAR(11.896, Utils::doublingTime(6), epsilon);
    EXPECT_NEAR(10.2448, Utils::doublingTime(7), epsilon);
    EXPECT_NEAR(9.006, Utils::doublingTime(8), epsilon);
    EXPECT_NEAR(8.043, Utils::doublingTime(9), epsilon);
    EXPECT_NEAR(7.273, Utils::doublingTime(10), epsilon);
    EXPECT_NEAR(3.802, Utils::doublingTime(20), epsilon);
    EXPECT_NEAR(2.642, Utils::doublingTime(30), epsilon);
    EXPECT_NEAR(2.060, Utils::doublingTime(40), epsilon);
    EXPECT_NEAR(1.710, Utils::doublingTime(50), epsilon);
    EXPECT_NEAR(1.475, Utils::doublingTime(60), epsilon);
    EXPECT_NEAR(1.306, Utils::doublingTime(70), epsilon);
}

TEST(Utils, pow)
{
    EXPECT_EQ(1, Utils::powi(0, 0));
    EXPECT_EQ(0, Utils::powi(0, 1));
    EXPECT_EQ(0, Utils::powi(0, 2));
    EXPECT_EQ(0, Utils::powi(0, 3));

    EXPECT_EQ(1, Utils::powi(1, 0));
    EXPECT_EQ(1, Utils::powi(1, 1));
    EXPECT_EQ(1, Utils::powi(1, 2));
    EXPECT_EQ(1, Utils::powi(1, 3));

    EXPECT_EQ(1, Utils::powi(2, 0));
    EXPECT_EQ(2, Utils::powi(2, 1));
    EXPECT_EQ(4, Utils::powi(2, 2));
    EXPECT_EQ(8, Utils::powi(2, 3));
}
