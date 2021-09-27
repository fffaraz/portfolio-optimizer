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

class CsvFile {
public:
    CsvFile(std::string filePath, bool hasHeader);

    using RecordType = std::vector<std::string>;
    using TableType = std::vector<RecordType>;

    std::string filePath() const noexcept { return m_filePath; }
    const RecordType& header() const noexcept { return m_header; }
    const TableType& data() const noexcept { return m_data; }

private:
    void loadFile(bool hasHeader);
    const std::string m_filePath;
    RecordType m_header;
    TableType m_data;
};

} // namespace Farazlib
