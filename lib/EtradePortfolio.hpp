/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#pragma once

#include "Portfolio.hpp"

#include <filesystem>

namespace Farazlib {

// Portfolio loaded from an Etrade CSV file
class EtradePortfolio : public Portfolio {
public:
    explicit EtradePortfolio(const std::filesystem::path& csvPath);
};

} // namespace Farazlib
