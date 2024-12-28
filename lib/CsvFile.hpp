/*
 * Copyright (c) 2021  Faraz Fallahi <fffaraz@gmail.com>
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file
 */

#pragma once

#include <string>
#include <vector>

namespace Farazlib {

// Read only CSV file loaded into memory
class CsvFile {
public:
    CsvFile(const std::string& filePath, bool hasHeader);

    using RowType = std::vector<std::string>; // list of columns in a row
    using TableType = std::vector<RowType>; // list of rows (each row is a list of columns)

    [[nodiscard]] const RowType& header() const noexcept { return m_header; }
    [[nodiscard]] const TableType& data() const noexcept { return m_data; }

private:
    RowType m_header; // list of columns in the header row
    TableType m_data; // list of rows (each row is a list of columns)
};

} // namespace Farazlib
