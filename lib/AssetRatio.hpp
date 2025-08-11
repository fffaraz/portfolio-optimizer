/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#pragma once

#include "Asset.hpp"

namespace portopt {

class AssetRatio {
public:
    AssetRatio(const Asset& asset1, const Asset& asset2);
    [[nodiscard]] size_t size() const noexcept;
    [[nodiscard]] const Ohlc& at(size_t i) const;

private:
    const Asset& m_asset1;
    const Asset& m_asset2;
};

} // namespace portopt
