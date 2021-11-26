/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

// https://www.bankrate.com/retirement/calculators/retirement-plan-calculator/

#include <QDateTime>

#include <iostream>

int main()
{
    auto current_age = 31;
    const auto retirement_age = 45;
    const auto death_age = 85;

    auto annual_income = 80'000;
    const auto income_increase = 3;
    const auto retirement_income = 50;

    const auto savings_percent = 50;
    auto current_savings = 200'000;

    const auto rate_of_return_before = 8;
    const auto rate_of_return_after = 4;

    while (current_age < retirement_age)
    {
        std::cout << "current_age: " << current_age++ << std::endl;
        current_savings += annual_income * (savings_percent / 100.0);
        current_savings *= (100.0 + rate_of_return_before) / 100.0;
        std::cout << "current_savings: " << current_savings << std::endl;
        annual_income *= (100.0 + income_increase) / 100.0;
        std::cout << "annual_income: " << annual_income << std::endl;
        std::cout << std::endl;
    }
    while (current_age < death_age)
    {
        std::cout << "current_age: " << current_age++ << std::endl;
        current_savings -= annual_income * (retirement_income / 100.0);
        current_savings *= (100.0 + rate_of_return_after) / 100.0;
        std::cout << "current_savings: " << current_savings << std::endl;
        annual_income *= (100.0 + income_increase) / 100.0;
        std::cout << "annual_income: " << annual_income << std::endl;
        std::cout << std::endl;
    }

    std::cout << "\nDONE" << std::endl;
    return 0;
}
