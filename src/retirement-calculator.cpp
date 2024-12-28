/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

// https://www.bankrate.com/retirement/calculators/retirement-plan-calculator/

#include <iostream>

int main()
{
    const auto current_age = 30;
    const auto retirement_age = 50;
    const auto social_security_age = 67;
    const auto death_age = 90;

    const auto monthly_savings = 1000; // stop at retirement age
    const auto monthly_expenses = 1000; // after retirement
    const auto social_security = 1000; // start at social security age
    auto current_savings = 100'000; // at current age

    const auto rate_of_return_before = 4; // before retirement (annual)
    const auto rate_of_return_after = 2; // after retirement (annual)

    std::cout << "age\tsavings\n";
    std::cout << current_age << "\t" << current_savings << "\n";

    auto current_age_month = current_age * 12;
    while (current_age_month < retirement_age * 12) {
        current_age_month++;
        current_savings *= (100.0 + rate_of_return_before / 12.0) / 100.0;
        current_savings += monthly_savings;
        std::cout << current_age_month / 12.0 << "\t" << current_savings << "\n";
    }

    // std::cout << "retirement starts\n";

    while (current_age_month < social_security_age * 12) {
        current_age_month++;
        if (current_savings > 0) {
            current_savings *= (100.0 + rate_of_return_after / 12.0) / 100.0;
        }
        current_savings -= monthly_expenses;
        std::cout << current_age_month / 12.0 << "\t" << current_savings << "\n";
    }

    // std::cout << "social security starts\n";

    while (current_age_month < death_age * 12) {
        current_age_month++;
        if (current_savings > 0) {
            current_savings *= (100.0 + rate_of_return_after / 12.0) / 100.0;
        }
        current_savings -= monthly_expenses;
        current_savings += social_security;
        std::cout << current_age_month / 12.0 << "\t" << current_savings << "\n";
    }

    std::cout << "\nDONE\n";
    return 0;
}
