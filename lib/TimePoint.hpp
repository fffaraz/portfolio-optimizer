/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#pragma once

#include <chrono>

namespace Farazlib {

// using TimePoint = std::chrono::year_month_day;
// using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
using TimePoint = std::chrono::system_clock::time_point;

} // namespace Farazlib
